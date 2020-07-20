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

using Keys = RC::Client::Controller::Network::Keys;

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
			try {
				pl.send_update(packet);
			} catch (...) {}
		}
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
	}

	void SGame::onPacketReceived(const void *data, size_t size, Server::Client &client)
	{
		const auto &pack = *reinterpret_cast<const Network::Packet *>(data);

		switch (pack.opcode) {
		case Network::GAME_INPUT:
			if (size < sizeof(Network::PacketInput))
				throw ::RC::Network::InvalidPacketSizeException(size, sizeof(Network::PacketInput));

			//TODO: Find which player sent it
			this->onKeys(0, pack.input.keys);
			break;
		default:
			throw Server::InvalidOpcodeException(pack.opcode);
		}
	}
}