/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** GameObject.hpp
*/
#ifndef RCONSOLE_GAMEOBJECT_HPP
#define RCONSOLE_GAMEOBJECT_HPP

#include "PongPackets.hpp"

namespace RC::Pong {
	struct Vector2 {
		int x;
		int y;

		operator Network::Position() const noexcept ;
	};

	class Racket;
	struct Ball {
		Vector2 pos;
		float speed;
		float angle;

		operator Network::Ball() const noexcept;
		void update(const Racket &r1, const Racket &r2) noexcept;
	};

	struct Racket {
		Vector2 pos;
		int size;
		Direction1D direction1D;
		float speed;

		operator Network::Racket() const noexcept;
	};
}


#endif //RCONSOLE_GAMEOBJECT_HPP
