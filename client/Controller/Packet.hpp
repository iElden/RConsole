//
// Created by Gegel85 on 14/07/2020.
//

#ifndef RCONSOLE_PACKET_HPP
#define RCONSOLE_PACKET_HPP


#define PACKED __attribute((packed))

namespace RC::Client::Controller::Network
{
	enum PACKED Opcode {
		HELLO,
		OLLEH,
		INPUT_REQ,
		INPUTS,
		GOODBYE
	};

	struct PACKED Keys {
		bool up: 1;
		bool down: 1;
		bool left: 1;
		bool right: 1;
		bool a: 1;
		bool b: 1;
		bool x: 1;
		bool y: 1;
		float axisX;
		float axisY;
		float axisZ;
	};

	struct PACKED InputReqPacket {
		Opcode opcode;
		bool vibrate;
	};

	struct PACKED InputPacket {
		Opcode opcode;
		Keys keys;
	};

	union PACKED Packet {
		Opcode opcode;
		InputPacket input;
		InputReqPacket inputReq;
	};
}


#endif //RCONSOLE_PACKET_HPP
