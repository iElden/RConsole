/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.cpp
*/

#include "SGame.hpp"

namespace RC::Pong
{
	SGame::SGame(const RC::Server::ClientList &players)
	{
		for (const std::shared_ptr<Server::Client> &p : players)
			this->players.emplace_back(p);
	}

	void SGame::run()
	{
		sf::Clock clock;

		while (true) {
			clock.restart();
			this->update();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICK_PER_SECOND - clock.getElapsedTime().asMilliseconds()));
		}
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
			Network::GAME_UPDATE
		};

		packet.ball = this->ball;
		packet.racket1 = this->racket1;
		packet.racket2 = this->racket2;
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