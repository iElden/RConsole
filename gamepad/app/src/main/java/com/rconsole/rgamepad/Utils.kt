package com.rconsole.rgamepad

import android.util.Log
import java.io.PrintWriter
import java.io.StringWriter

object Utils {

    fun err(tag: String, e: Exception) {
        val sw = StringWriter()
        e.printStackTrace(PrintWriter(sw))
        val exceptionAsString = sw.toString()
        Log.e(tag, exceptionAsString)
    }
}