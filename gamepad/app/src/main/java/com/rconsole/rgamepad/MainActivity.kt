package com.rconsole.rgamepad

import android.app.Activity
import android.content.Context
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.VibrationEffect
import android.os.Vibrator
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.Toast

class MainActivity : AppCompatActivity() {

    private var ip: String = ""
    private var id: Int = 0

    private fun vibrate() {
        val vibrator = this.getSystemService(Context.VIBRATOR_SERVICE) as Vibrator
        vibrator.vibrate(VibrationEffect.createOneShot(100, VibrationEffect.DEFAULT_AMPLITUDE))
    }

    private fun initGamepadButtons() {
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
                SendData(ip, id, button.toString()) {}.execute()
                vibrate()
            }))
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.register_activity)
    }

    fun connectToClient(view: View) {

        val inputIp = findViewById<EditText>(R.id.clientIP).text.toString() // Client IP address
        val inputId = findViewById<EditText>(R.id.clientID).text.toString() // Client Socket

        if (inputIp.isNotEmpty() && inputId.isNotEmpty()) {
            ip = inputIp
            id = inputId.toInt()

            SendData(ip, id, "Hello") {
                runOnUiThread {
                    setContentView(R.layout.gamepad_activity)
                    initGamepadButtons()
                }
            }.execute()

            // Send HELLO to the client using the ID
            // Wait for the response.
            // If waiting > 5s: error
            // Else : launch gamepad
            // Test entered id (s)

        } else {
            Toast.makeText(
                applicationContext,
                "You must enter the client IP and ID.",
                Toast.LENGTH_LONG
            ).show()
        }
    }

    override fun onStop() {
        super.onStop()
        SendData(ip, id, "stop") {}.execute()
    }
}