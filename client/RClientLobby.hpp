/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RClientLobby.hpp
*/
#ifndef RCONSOLE_RCLIENTLOBBY_HPP
#define RCONSOLE_RCLIENTLOBBY_HPP

#include <vector>
#include "RPlayer.hpp"

namespace RC {
	class RClientLobby {
	public:
		int id;
		std::vector<RPlayer> players;

		void addPlayer(RPlayer &player);
		void addPlayer(int id, std::string &name);
		void removePlayer(RPlayer &player);
		void removePlayer(int id);
	};
}


#endif //RCONSOLE_RCLIENTLOBBY_HPP