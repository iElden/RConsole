/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RClientLobby.hpp
*/
#ifndef RCONSOLE_LOBBY_HPP
#define RCONSOLE_LOBBY_HPP

#include <vector>
#include "Player.hpp"
#include "../network/RPackets.hpp"

namespace RC::Client
{
	class Lobby {
	public:
		size_t id;
		std::vector<Player> players;

		void addPlayer(Player &player);
		void addPlayer(int id, const std::string &name);
		void removePlayer(Player &player);
		void removePlayer(int id);
		operator Network::NLobby();

	};
}


#endif //RCONSOLE_LOBBY_HPP