/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.hpp
*/
#ifndef RCONSOLE_GAME_HPP
#define RCONSOLE_GAME_HPP

#include "GameObject.hpp"
#include "Player.hpp"
#include "../ISGame.hpp"

#define TICK_PER_SECOND 60

namespace RC::Pong {
	class Game : public ISGame {
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
		Game(const std::vector<std::shared_ptr<Server::Client>> &players);
		void update();
		void run() override;
	};
}


#endif //RCONSOLE_GAME_HPP
