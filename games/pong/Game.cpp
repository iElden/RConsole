/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.cpp
*/

#include "Game.hpp"


RC::Pong::Network::PacketUpdate RC::Pong::Game::get_packet()
{
	return RC::Pong::Network::PacketUpdate(this->ball, this->racket1, this->racket2);
}

void RC::Pong::Game::update()
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

void RC::Pong::Game::gameloop()
{
	this->ball.update(this->racket1, this->racket2);
}

void RC::Pong::Game::run()
{

}

RC::Pong::Game::Game(const std::vector<std::shared_ptr<RC::Server::Client>> &players)
{
	for (const std::shared_ptr<Server::Client> &p : players)
		this->players.emplace_back(p);
}

void RC::Pong::Game::send_update_to_player()
{
	Network::PacketUpdate packet = Network::PacketUpdate(this->ball, this->racket1, this->racket2);

	for (auto &pl : this->players) {
		pl.send_update(packet);
	}
}

void RC::Pong::Game::set_waiting_timer(unsigned int time)
{
	this->timer = time;
	this->gameState = WAITING;
}
