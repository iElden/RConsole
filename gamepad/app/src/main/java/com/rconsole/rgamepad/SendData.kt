package com.rconsole.rgamepad

import android.os.AsyncTask
import android.util.Log
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

class SendData internal constructor(private val ip: String, private val id: Int) :
    AsyncTask<Void?, Void?, Void?>() {
    override fun doInBackground(vararg v: Void?): Void? {
        try {
            val address = InetAddress.getByName(ip)
            val s = "Test."
            val msg = s.toByteArray()
            val packet = DatagramPacket(msg, msg.size, address, id)
            val socket = DatagramSocket()
            socket.send(packet)
            socket.close()
        } catch (e: Exception) {
            Log.e("BG", e.toString())
        }
        return null
    }
}