/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.hpp
*/
#ifndef RCONSOLE_SGAME_HPP
#define RCONSOLE_SGAME_HPP

#include "GameObject.hpp"
#include "Player.hpp"
#include "../ISGame.hpp"
#include "../../server/ClientList.hpp"

#define TICK_PER_SECOND 50

namespace RC::Pong {
	class SGame : public ISGame {
	private:
		std::vector<Player> players = {};

		Ball ball = {};
		Racket racket1 = {};
		Racket racket2 = {};
		enum GameState {
			WAITING,
			PLAYING,
			ENDED
		} gameState = WAITING;
		unsigned int timer = 0;

		void send_update_to_player();
		void set_waiting_timer(unsigned int time);
		void gameloop();

	public:
		SGame(const RC::Server::ClientList &players);
		void update();
		void run() override;
	};
}


#endif //RCONSOLE_SGAME_HPP
