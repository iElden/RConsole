/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RClientLobby.cpp
*/

#include "Lobby.hpp"

namespace RC::Client
{

	void Lobby::addPlayer(Player &player)
	{

	}

	void Lobby::addPlayer(int id, const std::string &name)
	{
		this->players.emplace_back(id, name);
	}

	void Lobby::removePlayer(Player &player)
	{

	}

	void Lobby::removePlayer(int id)
	{

	}

	Lobby::operator Network::NLobby()
	{
		return Network::NLobby();
	}
}