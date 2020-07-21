package com.rconsole.rgamepad

import android.content.Context
import android.os.Bundle
import android.os.VibrationEffect
import android.os.Vibrator
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.rconsole.rgamepad.Utils.Opcodes
import java.net.DatagramSocket
import java.net.InetAddress
import java.net.SocketTimeoutException
import kotlin.experimental.or


class MainActivity : AppCompatActivity() {

    private var lastId: Int = 0
    private var id: Int = 0
    private var ip: String = ""

    private var error: String = ""
    private var data = ByteArray(15)

    private lateinit var buttons: Array<Button>
    private lateinit var textIP: EditText
    private lateinit var textID: EditText
    private lateinit var connect: Button

    private var socket: DatagramSocket = DatagramSocket(null)
    private lateinit var address: InetAddress
    private var response: ByteArray = ByteArray(2)

    private fun vibrate() {
        val vibrator = this.getSystemService(Context.VIBRATOR_SERVICE) as Vibrator
        vibrator.vibrate(VibrationEffect.createOneShot(100, VibrationEffect.DEFAULT_AMPLITUDE))
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.gamepad_activity)

        this.socket.reuseAddress = true

        findViews()
        initButtons()
        setGamepadStatus(false)
    }

    private fun findViews() {
        this.buttons = arrayOf(
            findViewById(R.id.buttonA),
            findViewById(R.id.buttonB),
            findViewById(R.id.buttonX),
            findViewById(R.id.buttonY),
            findViewById(R.id.buttonUp),
            findViewById(R.id.buttonDown),
            findViewById(R.id.buttonLeft),
            findViewById(R.id.buttonRight)
        )
        this.textID = findViewById(R.id.clientID)
        this.textIP = findViewById(R.id.clientIP)
        this.connect = findViewById(R.id.connect)
    }

    private fun initButtons() {
        for (button in this.buttons) {
            button.setOnTouchListener { view, _ ->
                this.data[0] = this.data[0].or(button.tag.toString().toInt().toByte())
                view.performClick()
                button.performClick()
                return@setOnTouchListener true
            }
        }
    }

    private fun setGamepadStatus(enable: Boolean) {
        for (button in this.buttons) {
            button.isEnabled = enable
        }
        this.textIP.isEnabled = !enable
        this.textID.isEnabled = !enable
        this.connect.isEnabled = !enable
    }

    private fun getInput() {
        val inputIP = this.textIP.text.toString()
        val inputID = this.textID.text.toString()

        if (inputIP.isEmpty() || inputID.isEmpty()) {
            this.error = "You must fill IP and ID."
            throw Exception(this.error)
        }
        this.ip = inputIP
        this.id = inputID.toInt()
    }

    private fun initNetwork() {
        if (this.lastId != this.id) {
            this.socket = DatagramSocket(this.id)
            this.lastId = this.id
        }
        this.address = InetAddress.getByName(ip)
    }

    private fun send(packet: Packet) {
        Network.send(socket, packet, address, id)
    }

    private fun receive(timeout: Int = 2000): ByteArray {
        return Network.receive(socket, timeout)
    }

    private fun run() {
        try {
            runOnUiThread {
                findViewById<Button>(R.id.connect).isEnabled = false
            }
            getInput()
            initNetwork()
            send(Packet(Opcodes.HELLO))

            this.response = receive(10000)
            if (this.response[0] != Opcodes.OLLEH.byte) {
                throw Exception("Incorrect response. Try an other client.")
            }

            runOnUiThread {
                setGamepadStatus(true)
            }
            while (!this.socket.isClosed) {
                try {
                    this.response = receive()

                    if (this.response[0] == Opcodes.GOODBYE.byte) {
                        runOnUiThread {
                            setGamepadStatus(false)
                        }
                        break
                    }
                    if (this.response[0] == Opcodes.INPUT_REQ.byte) {
                        send(Packet(Opcodes.INPUTS, data))
                        data[0] = 0
                    }
                } catch (e: SocketTimeoutException) {

                } catch (e: Exception) {
                    Utils.err("LOOP", e)
                }
            }
            throw Exception("Disconnected from client.")

        } catch (e: Exception) {
            Utils.err("RUN", e)
            runOnUiThread {
                Toast.makeText(applicationContext, e.message, Toast.LENGTH_SHORT).show()
                setGamepadStatus(false)
            }
        }
    }

    fun connectToClient(_unused_: View) {
        Thread { run() }.start()
    }

    override fun onStop() {
        super.onStop()
        if (this::address.isInitialized) {
            Thread { send(Packet(Opcodes.GOODBYE)) }.start()
        }
        socket.close()
    }

    override fun onPause() {
        super.onPause()
        if (this::address.isInitialized) {
            Thread { send(Packet(Opcodes.GOODBYE)) }.start()
        }
    }

    override fun onResume() {
        super.onResume()
        setGamepadStatus(false)
    }
}