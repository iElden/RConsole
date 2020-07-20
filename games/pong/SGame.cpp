/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.cpp
*/

#include "SGame.hpp"

namespace RC::Pong
{
	SGame::SGame(const std::vector<std::shared_ptr<Server::Client>> &players)
	{
		for (const std::shared_ptr<Server::Client> &p : players)
			this->players.emplace_back(p);
	}

	void SGame::run()
	{

	}

	void SGame::update()
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

	void RC::Pong::SGame::gameloop()
	{
		this->ball.update(this->racket1, this->racket2);
		this->send_update_to_player();
	}

	void SGame::send_update_to_player()
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

	void SGame::set_waiting_timer(unsigned int time)
	{
		this->timer = time;
		this->gameState = WAITING;
	}
}