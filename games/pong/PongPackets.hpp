/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** PongPackets.hpp
*/

#ifndef RCONSOLE_PONGPACKETS_HPP
#define RCONSOLE_PONGPACKETS_HPP

#include "Utils.hpp"

#define PACKED __attribute((packed))

namespace RC::Pong::Network {

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
		int pos;
		int size;
		Direction1D direction;
		float speed;
	};

	struct PACKED PacketUpdate {
		Ball ball;
		Racket racket1;
		Racket racket2;
		PacketUpdate(const Ball &b, const Racket &r1, const Racket &r2):
			ball(b), racket1(r1), racket2(r2){};
	};

	struct PACKED PacketScore {
		int score_1;
		int score_2;
	};
}

#endif //RCONSOLE_PONGPACKETS_HPP
