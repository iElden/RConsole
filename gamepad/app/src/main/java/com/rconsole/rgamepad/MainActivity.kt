package com.rconsole.rgamepad

import android.content.Context
import android.net.ConnectivityManager
import android.net.wifi.WifiManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.VibrationEffect
import android.os.Vibrator
import android.text.format.Formatter
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import org.jetbrains.anko.doAsync
import org.jetbrains.anko.doAsyncResult
import java.io.IOException
import java.lang.Exception
import java.lang.ref.WeakReference
import java.net.InetAddress

class MainActivity : AppCompatActivity() {

    private fun getButton(id: Int): Button {
        return findViewById(id)
    }


    private fun vibrate() {
        val vibrator = this.getSystemService(Context.VIBRATOR_SERVICE) as Vibrator
        vibrator.vibrate(VibrationEffect.createOneShot(100, VibrationEffect.DEFAULT_AMPLITUDE))
    }

    private fun initGamepadButtons() {
        val buttons = arrayOf(
            getButton(R.id.buttonA),
            getButton(R.id.buttonB),
            getButton(R.id.buttonX),
            getButton(R.id.buttonY),
            getButton(R.id.buttonUp),
            getButton(R.id.buttonDown),
            getButton(R.id.buttonLeft),
            getButton(R.id.buttonRight)
        )

        for (button in buttons) {
            button.setOnTouchListener(Repeater(400, 100, View.OnClickListener {
                Log.i("Test", "$button")
                vibrate()
            }))
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.register_activity)
    }

    fun connectToClient(view: View) {

        val ip = findViewById<EditText>(R.id.clientIP) // Client IP address
        val id = findViewById<EditText>(R.id.clientID) // Client Socket

        if (id.length() != 0 && ip.length() != 0) {
            SendData(ip.text.toString(), id.text.toString().toInt()).execute()
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
}