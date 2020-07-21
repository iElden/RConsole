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
#include "../../client/Controller/IController.hpp"

namespace RC::Pong {
	class SGame : public ISGame {
	private:
		std::vector<Player> players = {};

		Ball ball{{500, 250}, 7.5, 0};
		Racket racket1{{50, 200}, 100, 0, NONE, 10.f};
		Racket racket2{{950, 200}, 100, 0, NONE, 10.f};
		int score1 = 0;
		int score2 = 0;
		enum GameState {
			WAITING,
			PLAYING,
			ENDED
		} gameState = WAITING;
		unsigned int timer = 0;

		void send_update_to_player();
		void send_score_to_player();
		void set_waiting_timer(unsigned int time);
		void goal(int pl);
		void gameloop();

	public:
		SGame(const RC::Server::ClientList &players);
		void update();
		void run() override;
		void onKeys(int player, Client::Controller::Network::Keys keys);
		void onPacketReceived(const void *data, size_t size, Server::Client &client) override;
		int getPlayerIDByClient(const Server::Client &client);
	};
}


#endif //RCONSOLE_SGAME_HPP
