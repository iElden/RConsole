package com.rconsole.rgamepad

import android.content.Context
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.VibrationEffect
import android.os.Vibrator
import android.util.Log
import android.view.View
import android.widget.Button

class MainActivity : AppCompatActivity() {

    private fun getButton(id: Int) : Button {
        return findViewById(id)
    }


    private fun vibrate() {
        val vibrator = this.getSystemService(Context.VIBRATOR_SERVICE) as Vibrator
        vibrator.vibrate(VibrationEffect.createOneShot(100, VibrationEffect.DEFAULT_AMPLITUDE))
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val buttons = arrayOf(getButton(R.id.buttonA), getButton(R.id.buttonB), getButton(R.id.buttonX), getButton(R.id.buttonY),
            getButton(R.id.buttonUp), getButton(R.id.buttonDown), getButton(R.id.buttonLeft), getButton(R.id.buttonRight))

        for (button in buttons) {
            button.setOnTouchListener(Repeater(400, 100, View.OnClickListener {
                Log.i("Test", "$button")
                vibrate()
            }))
        }

    }
}