package com.rconsole.rgamepad

import android.content.Context
import android.os.Bundle
import android.os.VibrationEffect
import android.os.Vibrator
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.io.PrintWriter
import java.io.StringWriter
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress


class MainActivity : AppCompatActivity() {

    private var ip: String = ""
    private var id: Int = 0
    private lateinit var address: InetAddress
    private var lastID = ""

    private var send: DatagramSocket? = null
    //private var receive: DatagramSocket? = null


    private fun vibrate() {
        val vibrator = this.getSystemService(Context.VIBRATOR_SERVICE) as Vibrator
        vibrator.vibrate(VibrationEffect.createOneShot(100, VibrationEffect.DEFAULT_AMPLITUDE))
    }

    private fun initGamepadButtons(s: DatagramSocket?) {
        val buttons = arrayOf<Button>(
            findViewById(R.id.buttonA),
            findViewById(R.id.buttonB),
            findViewById(R.id.buttonX),
            findViewById(R.id.buttonY),
            findViewById(R.id.buttonUp),
            findViewById(R.id.buttonDown),
            findViewById(R.id.buttonLeft),
            findViewById(R.id.buttonRight)
        )

        for (button in buttons) {
            button.setOnTouchListener(Repeater(400, 100, View.OnClickListener {
                Thread {
                    send(button.toString())
                }.start()
                vibrate()
            }))
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.register_activity)
    }

    private fun receive(timeout: Int = 2000): String {
        val buffer = ByteArray(1000)
        val packet = DatagramPacket(buffer, 0, buffer.size)
        //receive?.soTimeout = timeout
        send?.soTimeout = timeout
        //receive?.receive(packet)
        send?.receive(packet)
        return String(buffer, 0, buffer.size)
    }

    private fun send(msg: String) {
        val bytes = msg.toByteArray()
        val packet = DatagramPacket(bytes, bytes.size, address, id)
        send?.send(packet)
    }

    private fun firstSend() {
        send("Hello")
    }

    fun err(e: Exception) {
        val sw = StringWriter()
        e.printStackTrace(PrintWriter(sw))
        val exceptionAsString = sw.toString()
        Log.e("Thread", exceptionAsString)
    }

    private fun run() {
        val btn = findViewById<Button>(R.id.connect)

        val inputIp = findViewById<EditText>(R.id.clientIP).text.toString() // Client IP address
        val inputId = findViewById<EditText>(R.id.clientID).text.toString() // Client Socket

        if (inputIp.isNotEmpty() && inputId.isNotEmpty()) {
            ip = inputIp
            id = inputId.toInt()

            try {
                if (lastID != inputId) {
                    send?.close()
                    //receive?.close()
                    send = DatagramSocket(id)
                    //receive = DatagramSocket(id + 1)
                    lastID = inputId
                }

                address = InetAddress.getByName(ip)

                firstSend()

                runOnUiThread {
                    btn.isEnabled = false
                }
                val msg = receive()
                runOnUiThread {
                    btn.isEnabled = true
                }

                if (msg.startsWith("Olleh")) {
                    runOnUiThread {
                        setContentView(R.layout.gamepad_activity)
                        initGamepadButtons(send)
                    }
                } else {
                    throw RException("Bad response, try an other client.")
                }

            } catch (e: Exception) {
                err(e)
                runOnUiThread {
                    Toast.makeText(applicationContext, e.toString(), Toast.LENGTH_SHORT).show()
                    btn.isEnabled = true
                }
            }
        } else {
            runOnUiThread {
                Toast.makeText(
                    applicationContext,
                    "You must enter the client IP and ID.",
                    Toast.LENGTH_LONG
                ).show()
            }
        }
    }

    fun connectToClient(view: View) {
        try {
            Thread {
                run()
            }.start()
        } catch (e: Exception) {}
    }

    override fun onStop() {
        send?.close()
        //receive?.close()
        super.onStop()
    }
}