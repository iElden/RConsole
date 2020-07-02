package com.rconsole.rgamepad

import android.os.AsyncTask
import android.util.Log
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

class SendData(private var ip: String, private var id: Int, private var msg: String, private var f: () -> Unit) : AsyncTask<Void?, Void?, Void?>() {
    override fun doInBackground(vararg params: Void?): Void? {
        try {
            val address = InetAddress.getByName(ip)
            val bytes = msg.toByteArray()
            val packet = DatagramPacket(bytes, bytes.size, address, id)
            val socket = DatagramSocket()
            socket.send(packet)
            socket.close()
            f()
        } catch (e: Exception) {
            Log.e("BG", e.toString())
        }
        return null
    }
}