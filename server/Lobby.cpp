/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Lobby.cpp
*/

#include "Lobby.hpp"

namespace RC::Server {
	Lobby::Lobby(uint32_t id, std::shared_ptr<Client> &owner) :
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

		for (std::shared_ptr<Client> &player : this->players)
			result.push_back(player->getNPlayer());
		return result;
	}
}
