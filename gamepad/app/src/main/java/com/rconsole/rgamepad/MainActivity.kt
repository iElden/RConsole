package com.rconsole.rgamepad

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.os.Bundle
import android.os.VibrationEffect
import android.os.Vibrator
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.net.DatagramSocket
import java.net.InetAddress
import java.nio.ByteBuffer
import kotlin.experimental.and
import kotlin.experimental.or


class MainActivity : AppCompatActivity(), SensorEventListener /* See for custom SensorEventListener */ {

    private var ip: String = ""
    private var id: Int = 0
    private lateinit var address: InetAddress
    private var lastID = ""
    private var data = ByteArray(15)

    private lateinit var sensorManager: SensorManager
    private lateinit var gyroscope: Sensor
    private lateinit var accelerometer: Sensor
    private lateinit var gameRotationVector: Sensor

    private var socket: DatagramSocket? = null

    private fun vibrate() {
        val vibrator = this.getSystemService(Context.VIBRATOR_SERVICE) as Vibrator
        vibrator.vibrate(VibrationEffect.createOneShot(100, VibrationEffect.DEFAULT_AMPLITUDE))
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        this.sensorManager = getSystemService(Context.SENSOR_SERVICE) as SensorManager
        sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE)?.let {
            this.gyroscope = it
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)?.let {
            this.accelerometer = it
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_GAME_ROTATION_VECTOR)?.let {
            this.gameRotationVector = it
        }

        super.onCreate(savedInstanceState)
        setContentView(R.layout.register_activity)
    }

    // May be correct. :x
    private fun same(byte: Byte, other: Byte): Boolean {
        return byte.and(other) == other
    }

    private fun initButtons() {
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
                this.data[0] = this.data[0].or(button.tag.toString().toInt().toByte())
                //packet[0] = packet[0].or(button.tag.toString().toInt().toByte())
                view.performClick()
                button.performClick()
                return@setOnTouchListener true
            }
        }
    }

    private fun gamepadLoop() {
        Thread {
            initButtons()
            while (true) {
                try {
                    var msg = Network.receive(socket,10000)

                    if (same(msg[0], 0x02)) {
                        if (same(msg[1], 0x00)) {
                            vibrate()
                        }
                    }
                    Network.send(socket, Packet(3.toByte(), data), address, id)
                    data[0] = 0
                } catch (e: Exception) { Utils.err("TH-GamepadLoop", e) }
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

                Network.send(socket, Packet(0.toByte()), address, id)

                runOnUiThread { btn.isEnabled = false }
                val msg = Network.receive(socket)
                runOnUiThread { btn.isEnabled = true }

                if (same(msg[0], 0x01)) {
                    runOnUiThread {
                        setContentView(R.layout.gamepad_activity)
                        gamepadLoop()
                    }
                } else { throw RException("Bad response, try an other client.") }
            } catch (e: Exception) {
                Utils.err("TH-run", e)
                runOnUiThread {
                    Toast.makeText(applicationContext, e.toString(), Toast.LENGTH_SHORT).show()
                    btn.isEnabled = true
                }
            }
        } else { runOnUiThread { Toast.makeText(applicationContext, "You must enter the client IP and ID.", Toast.LENGTH_LONG).show() } }
    }

    fun connectToClient(_unused_: View) {
        try {
            Thread { run() }.start()
        } catch (e: Exception) { }
    }

    override fun onStop() {
        this.sensorManager.unregisterListener(this)
        socket?.close()
        super.onStop()
    }

    override fun onResume() {
        this.sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL)
        this.sensorManager.registerListener(this, gyroscope, SensorManager.SENSOR_DELAY_NORMAL)
        this.sensorManager.registerListener(this, gameRotationVector, SensorManager.SENSOR_DELAY_NORMAL)
        super.onResume()
    }

    override fun onAccuracyChanged(p0: Sensor?, p1: Int) {}

    // Float to byte to change and sensor type.
    override fun onSensorChanged(event: SensorEvent?) {
        when (event?.sensor?.type) {
            Sensor.TYPE_GAME_ROTATION_VECTOR -> {
                var x = event.values[0]
                var y = event.values[1]
                var z = event.values[2]

                Log.i("X", x.toString())
                Log.i("Y", y.toString())
                Log.i("Z", z.toString())

                var xbyte = ByteBuffer.allocate(4).putFloat(x).array()
                var ybyte = ByteBuffer.allocate(4).putFloat(y).array()
                var zbyte = ByteBuffer.allocate(4).putFloat(z).array()

                for (i in 0..3) {
                    data[i + 1] = xbyte[i]
                    data[i + 5] = ybyte[i]
                    data[i + 9] = zbyte[i]
                }
            }
        }
    }
}