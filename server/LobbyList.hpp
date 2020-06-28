/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** LobbyList.hpp
*/
#ifndef RCONSOLE_LOBBYLIST_HPP
#define RCONSOLE_LOBBYLIST_HPP

#include <vector>
#include <memory>
#include "Lobby.hpp"

namespace RC::Server {
	class LobbyList {
	private:
		std::vector<std::shared_ptr<Lobby>> _lobbies = {};

		// Overload Iterator
		auto begin() {return this->_lobbies.begin();};
		auto end() {return this->_lobbies.end();};
	};
};


#endif //RCONSOLE_LOBBYLIST_HPP
