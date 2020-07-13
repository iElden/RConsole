package com.rconsole.rgamepad

data class Packet(val opcode: Byte, val data: ByteArray = ByteArray(0)) {
}