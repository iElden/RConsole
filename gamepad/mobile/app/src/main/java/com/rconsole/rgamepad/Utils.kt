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

    enum class Opcodes(val byte: Byte) {
        HELLO(0),
        OLLEH(1),
        INPUT_REQ(2),
        INPUTS(3),
        GOODBYE(4)
    }

}