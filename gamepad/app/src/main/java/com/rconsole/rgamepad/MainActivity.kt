package com.rconsole.rgamepad

import android.content.Context
import android.os.Bundle
import android.os.VibrationEffect
import android.os.Vibrator
import android.util.Log
import android.view.MotionEvent
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
import kotlin.experimental.and
import kotlin.experimental.or
import kotlin.experimental.xor


class MainActivity : AppCompatActivity() {

    private var ip: String = ""
    private var id: Int = 0
    private lateinit var address: InetAddress
    private var lastID = ""

    private var socket: DatagramSocket? = null

    private fun vibrate() {
        val vibrator = this.getSystemService(Context.VIBRATOR_SERVICE) as Vibrator
        vibrator.vibrate(VibrationEffect.createOneShot(100, VibrationEffect.DEFAULT_AMPLITUDE))
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.register_activity)
    }

    private fun receive(timeout: Int = 2000): ByteArray {
        val buffer = ByteArray(2)
        val packet = DatagramPacket(buffer, 0, buffer.size)
        socket?.soTimeout = timeout
        socket?.receive(packet)
        return buffer
    }

    private fun send(packet: Packet) {
        var msg = ByteArray(1)
        msg[0] = packet.opcode
        msg = msg.plus(packet.data)
        socket?.send(DatagramPacket(msg, msg.size, address, id))
    }

    private fun firstSend() {
        send(Packet(0.toByte()))
    }

    private fun err(e: Exception) {
        val sw = StringWriter()
        e.printStackTrace(PrintWriter(sw))
        val exceptionAsString = sw.toString()
        Log.e("Thread", exceptionAsString)
    }


    fun same(byte: Byte, mask: Byte): Boolean {
        return byte.and(mask) == mask
    }

    private fun gamepadLoop() {

        Thread {
            var packet = ByteArray(14)
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
                button.setOnTouchListener { view, me ->
                    packet[0] = packet[0].or(button.tag.toString().toInt().toByte())
                    return@setOnTouchListener true
                }
            }
            while (true) {
                try {
                    var msg = receive(10000)

                    if (same(msg[0], 0x02)) {
                        if (same(msg[1], 0x00)) {
                            vibrate() // Have to play with bit manipulation.
                        }
                    }
                    send(Packet(3.toByte(), packet))
                    packet = ByteArray(14)
                } catch (e: Exception) {
                    err(e)
                }
            }
        }.start()
    }

    private fun run() {
        val btn = findViewById<Button>(R.id.connect)

        val inputIp = findViewById<EditText>(R.id.clientIP).text.toString() // Client IP address
        val inputId = findViewById<EditText>(R.id.clientID).text.toString() // Client Socket

        if (inputIp.isNotEmpty() && inputId.isNotEmpty()) {
            try {
                ip = inputIp
                id = inputId.toInt()

                if (lastID != inputId || socket?.isClosed!!) {
                    socket?.close()
                    socket = DatagramSocket(id)
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
                if (same(msg[0], 0x01)) {
                    runOnUiThread {
                        setContentView(R.layout.gamepad_activity)
                        gamepadLoop()
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
        } catch (e: Exception) {
        }
    }

    override fun onStop() {
        socket?.close()
        super.onStop()
    }
}