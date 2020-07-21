/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.cpp
*/

#include <iostream>
#include "SGame.hpp"
#include "../../network/Exceptions.hpp"
#include "../../server/exc.hpp"
#include "macro.hpp"

using Keys = RC::Client::Controller::Network::Keys;

namespace RC::Pong
{
	SGame::SGame(const RC::Server::ClientList &players)
	{
		for (const std::shared_ptr<Server::Client> &p : players)
			this->players.emplace_back(p);
		srand(time(nullptr));
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
				break;
			case PLAYING:
				this->gameloop();
				break;
			case ENDED:
				break;
		}
		this->send_update_to_player();
	}

	void RC::Pong::SGame::gameloop()
	{
		this->ball.update(this->racket1, this->racket2);
		if (this->ball.pos.x <= 0)
			this->goal(1);
		if (this->ball.pos.x >= PONG_MAX_X)
			this->goal(0);
		if (this->racket1.striked) {
			this->racket2.ult -= 5;
			if (this->racket2.ult < 0)
				this->racket1.unstrike();
		}
		if (this->racket2.striked)
			this->racket1.ult -= 5;
			if (this->racket1.ult < 0)
				this->racket2.unstrike();
		this->racket1.update();
		this->racket2.update();
	}

	void RC::Pong::SGame::goal(int pl)
	{
		(pl ? this->score2 : this->score1) += 1;
		this->ball.reset();
		this->set_waiting_timer(3 * TICK_PER_SECOND);
		this->send_score_to_player();
	}

	void SGame::set_waiting_timer(unsigned int time)
	{
		this->timer = time;
		this->gameState = WAITING;
	}

	void SGame::onKeys(int player, Client::Controller::Network::Keys keys)
	{
		if (player >= 2)
			return;

		Racket &player_racket = (player ? this->racket2 : this->racket1);
		if (keys.up)
			player_racket.move(UP);
		else if (keys.down)
			player_racket.move(DOWN);
		else
			player_racket.move(NONE);

		if (keys.x) {
			if (player_racket.ult >= 700) {
				player_racket.ult -= 700;
				this->ball.set_slowed();
			}
		}
		if (keys.y) {
			if (player_racket.ult >= 1000) {
				player_racket.ult = 975;
				player_racket.boost();
			}
		}
		if (keys.a) {
			if (player_racket.ult >= 1000) {
				(player ? this->racket1 : this->racket2).strike();
				player_racket.ult = 975;
			}
		}

		player_racket.sprinting = keys.b;
	}

	void SGame::onPacketReceived(const void *data, size_t size, Server::Client &client)
	{
		const auto &pack = *reinterpret_cast<const Network::Packet *>(data);

		switch (pack.opcode) {
		case Network::GAME_INPUT:
			if (size < sizeof(Network::PacketInput))
				throw ::RC::Network::InvalidPacketSizeException(size, sizeof(Network::PacketInput));

			this->onKeys(this->getPlayerIDByClient(client), pack.input.keys);
			break;
		default:
			throw Server::InvalidOpcodeException(pack.opcode);
		}
	}

	int SGame::getPlayerIDByClient(const Server::Client &client)
	{
		for (int i = 0; i < this->players.size(); i++) {
			if (this->players[i] == client)
				return i;
		}
		return 2147483647;
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
			try {
				pl.send_update(packet);
			} catch (...) {}
		}
	}

	void SGame::send_score_to_player()
	{
		Network::PacketScore packet{
				Network::SCORE
		};

		packet.score_1 = this->score1;
		packet.score_2 = this->score2;
		for (auto &pl : this->players) {
			try {
				pl.send_score(packet);
			} catch (...) {}
		}
	}
}