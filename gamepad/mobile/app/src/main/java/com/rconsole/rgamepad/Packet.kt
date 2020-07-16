package com.rconsole.rgamepad

data class Packet(val opcode: Utils.Opcodes, val data: ByteArray = ByteArray(0)) {
}