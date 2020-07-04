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
                SendData(ip, id).execute(AsyncParams(button.toString(), applicationContext ))
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

            SendData(ip, id, after = {
                if (it != null) {
                    Toast.makeText(applicationContext, it.toString(), Toast.LENGTH_SHORT).show()
                } else {
                    ReceiveData(id, after = {
                        runOnUiThread {
                            setContentView(R.layout.gamepad_activity)
                            initGamepadButtons()
                        }
                    }).execute()
                }
            }).execute(AsyncParams("Hello", applicationContext))
        } else {
            Toast.makeText(
                applicationContext,
                "You must enter the client IP and ID.",
                Toast.LENGTH_LONG
            ).show()
        }
    }

    override fun onStop() {
        SendData(ip, id).execute(AsyncParams("Stop", applicationContext))
        super.onStop()
    }
}