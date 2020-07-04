package com.rconsole.rgamepad

import android.content.Context
import android.net.ConnectivityManager
import android.os.AsyncTask
import android.util.Log
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress


/**
@param before The first line to be executed in this function (socket is not opened).
@param after The last line to be executed in this function (socket is closed).
 */
class SendData(
    private var ip: String,
    private var id: Int,
    private var before: (() -> Unit)? = null,
    private var after: ((Any?) -> Unit)? = null
) : AsyncTask<AsyncParams, Void?, Any?>() {

    var socket: DatagramSocket = DatagramSocket(id)

    override fun onPreExecute() {
        before?.invoke()
    }

    override fun onPostExecute(result: Any?) {
        after?.invoke(result)
    }

    override fun doInBackground(vararg params: AsyncParams): Any? {
        try {
            if (params[0].context != null) {
                val connectivityManager =
                    params[0].context?.getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager?
                val activeNetworkInfo = connectivityManager!!.activeNetworkInfo
                if (activeNetworkInfo == null || !activeNetworkInfo.isConnected) {
                    throw RException("Check your connection.")
                }
            }

            val address = InetAddress.getByName(ip)
            /*if (!address.isReachable(2000)) {
                throw RException("Client unreachable, check if it is connected and verify your inputs.")
            }*/
            println(params[0].msg)
            val bytes = params[0].msg.toByteArray()
            val packet = DatagramPacket(bytes, bytes.size, address, id)
            socket.send(packet)
            socket.close()
        } catch (e: Exception) {
            Log.e("SendDataBG", e.toString())
            return e.message
        }
        return null
    }

}