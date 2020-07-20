/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.cpp
*/

#include "Game.hpp"

namespace RC::Pong
{
	Game::Game(const std::vector<std::shared_ptr<Server::Client>> &players)
	{
		for (const std::shared_ptr<Server::Client> &p : players)
			this->players.emplace_back(p);
	}

	void Game::update()
	{
		switch (this->gameState) {
			case WAITING:
				if (!this->timer)
					this->gameState = PLAYING;
				else
					this->timer--;
				return;
			case PLAYING:
				this->gameloop();
				return;
			case ENDED:
				return;
		}
	}

	void Game::gameloop()
	{
		this->ball.update(this->racket1, this->racket2);
	}

	void Game::run()
	{

	}

	void Game::send_update_to_player()
	{
		Network::PacketUpdate packet{
			Network::GAME_UPDATE,
			this->ball,
			this->racket1,
			this->racket2
		};

		for (auto &pl : this->players) {
			pl.send_update(packet);
		}
	}

	void Game::set_waiting_timer(unsigned int time)
	{
		this->timer = time;
		this->gameState = WAITING;
	}
}