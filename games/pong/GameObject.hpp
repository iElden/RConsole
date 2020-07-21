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
		bool slowed;
		int slowed_timer;

		operator Network::Ball() const noexcept;
		void update(const Racket &r1, const Racket &r2) noexcept;
		void reset(float speed=7.5);
		void set_slowed();
	};

	struct Racket {
		Vector2 pos;
		int size;
		int ult;
		Direction1D direction1D;
		float speed;
		bool boosted;
		bool striked;
		bool sprinting;

		operator Network::Racket() const noexcept;
		void move(Direction1D new_dir);
		void addsize(int size);
		void boost();
		void unboost();
		void update();
		void strike();
		void unstrike();
	};
}


#endif //RCONSOLE_GAMEOBJECT_HPP
