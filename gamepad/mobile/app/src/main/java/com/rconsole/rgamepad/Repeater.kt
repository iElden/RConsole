package com.rconsole.rgamepad


import android.os.Handler
import android.view.MotionEvent
import android.view.View

class Repeater(
    private var initialDelay: Long,
    private var normalDelay: Long,
    private var clickListener: View.OnClickListener
) : View.OnTouchListener {

    private var handler = Handler()
    private var touchedView: View? = null

    private lateinit var handlerRunnable:Runnable

    override fun onTouch(v: View?, me: MotionEvent?): Boolean {
        touchedView = v
        handlerRunnable = Runnable {
            if (touchedView == null)
                return@Runnable
            if (!touchedView!!.isEnabled) {
                handler.removeCallbacks(handlerRunnable)
            } else {
                clickListener.onClick(touchedView)
                handler.postDelayed(handlerRunnable, normalDelay)
            }
        }
        when (me?.action) {
            MotionEvent.ACTION_DOWN -> {
                handler.removeCallbacks(handlerRunnable)
                handler.postDelayed(handlerRunnable, initialDelay)
                touchedView?.isPressed = true
                clickListener.onClick(touchedView)
                return true
            }
            MotionEvent.ACTION_UP -> {
                handler.removeCallbacks(handlerRunnable)
                touchedView?.isPressed = false
                touchedView = null
                return true
            }
            MotionEvent.ACTION_CANCEL -> {
                handler.removeCallbacks(handlerRunnable)
                touchedView?.isPressed = false
                touchedView = null
                return true
            }
        }
        return false
    }

}

