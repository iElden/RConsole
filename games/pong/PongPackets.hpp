/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** PongPackets.hpp
*/

#ifndef RCONSOLE_PONGPACKETS_HPP
#define RCONSOLE_PONGPACKETS_HPP

#include "Utils.hpp"
#include "../../client/Controller/Packet.hpp"

#define PACKED __attribute((packed))

namespace RC::Pong::Network
{
	enum PACKED Opcode {
		GAME_UPDATE,
		SCORE,
		GAME_INPUT
	};

	struct PACKED Position {
		int x;
		int y;
	};

	struct PACKED Ball {
		Position pos;
		float speed;
		float angle;
	};

	struct PACKED Racket {
		Position pos;
		int size;
		Direction1D direction;
		float speed;
	};

	struct PACKED PacketUpdate {
		Opcode opcode;
		Ball ball;
		Racket racket1;
		Racket racket2;
	};

	struct PACKED PacketScore {
		Opcode opcode;
		int score_1;
		int score_2;
	};

	struct PACKED PacketInput {
		Opcode opcode;
		Client::Controller::Network::Keys keys;
	};

	union PACKED Packet {
		Opcode opcode;
		PacketScore score;
		PacketUpdate update;
		PacketInput input;
	};
}

#endif //RCONSOLE_PONGPACKETS_HPP
