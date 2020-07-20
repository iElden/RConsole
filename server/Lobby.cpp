/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Lobby.cpp
*/

#include "Lobby.hpp"
#include "exc.hpp"
#include "../games/pong/SGame.hpp"

namespace RC::Server {
	Lobby::Lobby(uint32_t id, const std::shared_ptr<Client> &owner) :
		id(id),
		players(owner)
	{}

	RC::Network::NLobby Lobby::toNLobby()
	{
		return Network::NLobby{this->id};
	}

	std::vector<RC::Network::NPlayer> Lobby::getNPlayers()
	{
		std::vector<Network::NPlayer> result;

		for (const std::shared_ptr<Client> &player : this->players)
			result.push_back(static_cast<Network::NPlayer>(*player));
		return result;
	}

	Lobby &Lobby::join(const std::shared_ptr<Client> &client)
	{
		this->players.add(client);
		return *this;
	}

	void Lobby::leave(Client &client)
	{
		this->players.remove(client);
	}

	void Lobby::startGame(Network::GameID game_ID)
	{
		this->state = IN_GAME;


		if (game_ID == Network::GAME_ID_PONG)
			this->game.reset(new RC::Pong::SGame(this->players));
		for (const std::shared_ptr<Client> &cl : this->players)
			cl->connection.sendGameStart(game_ID);
		this->game_thread = std::thread([this]{this->game->run();});
	}
}
