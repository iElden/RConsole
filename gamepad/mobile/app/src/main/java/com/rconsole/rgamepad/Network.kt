package com.rconsole.rgamepad

import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

object Network {

    fun receive(socket: DatagramSocket?, timeout: Int): ByteArray {
        val buffer = ByteArray(2)
        val packet = DatagramPacket(buffer, 0, buffer.size)
        socket?.soTimeout = timeout
        if (!socket?.isClosed!!) {
            socket.receive(packet)
        }
        return buffer
    }

    fun send(socket: DatagramSocket?, packet: Packet, address: InetAddress, id: Int) {
        var msg = ByteArray(1)
        msg[0] = packet.opcode.byte
        msg = msg.plus(packet.data)
        if (!socket?.isClosed!!) {
            socket.send(DatagramPacket(msg, msg.size, address, id))
        }
    }
}