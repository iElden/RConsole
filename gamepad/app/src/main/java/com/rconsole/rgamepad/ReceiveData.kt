package com.rconsole.rgamepad

import android.os.AsyncTask
import android.util.Log
import java.net.DatagramPacket
import java.net.DatagramSocket

class ReceiveData(private var id: Int, private var before: (() -> Unit)? = null, private var after: ((String?) -> Unit)? = null) : AsyncTask<Void?, Void?, String?>() {

    override fun onPreExecute() {
        before?.invoke()
    }

    override fun onPostExecute(result: String?) {
        after?.invoke(result)
    }

    override fun doInBackground(vararg void: Void?): String? {
        var msg = ""

        try {
            val socket = DatagramSocket(id + 1)
            val buffer = ByteArray(1000)
            val packet = DatagramPacket(buffer, buffer.size, id + 1)

            socket.receive(packet)
            msg = String(buffer, 0, buffer.size)
            socket.close()
        } catch (e: Exception) {
            Log.e("ReceiveDataBG", e.toString())
            return null
        }
        return msg
    }
}