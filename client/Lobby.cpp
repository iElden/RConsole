/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RClientLobby.cpp
*/

#include "Lobby.hpp"

namespace RC::Client
{

	void Lobby::addPlayer(int id, const std::string &name)
	{
		if (std::find_if(this->players.begin(),this->players.end(),
			[id](Player &player){
				return player.getId() == id;
			}
		) == this->players.end())
			this->players.emplace_back(id, name);
	}

	void Lobby::removePlayer(int id)
	{
		this->players.erase(
			std::remove_if(
				this->players.begin(),
				this->players.end(),
				[id](Player &player){
					return player.getId() == id;
				}
			),
			this->players.end()
		);
	}

	Lobby::operator Network::NLobby()
	{
		return Network::NLobby();
	}
}