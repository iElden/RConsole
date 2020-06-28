/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Lobby.hpp
*/
#ifndef RCONSOLE_LOBBY_HPP
#define RCONSOLE_LOBBY_HPP

#include "ClientList.hpp"

namespace RC::Server {
	enum LobbyState {
		WAITING_FOR_PLAYER,
		CHOOSING_GAME,
		IN_GAME
	};

	class Lobby {
		ClientList players;
		LobbyState state;
	};
}


#endif //RCONSOLE_LOBBY_HPP
