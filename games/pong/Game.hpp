/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.hpp
*/
#ifndef RCONSOLE_GAME_HPP
#define RCONSOLE_GAME_HPP

#include "GameObject.hpp"

namespace RC::Pong {
	class Game {
	private:
		Ball ball;
		Racket racket1;
		Racket racket2;

		Network::PacketUpdate get_packet();

	public:
		Game();
		void update();
		void run();
	};
}


#endif //RCONSOLE_GAME_HPP
