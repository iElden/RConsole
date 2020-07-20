/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Lobby.hpp
*/
#ifndef RCONSOLE_LOBBY_HPP
#define RCONSOLE_LOBBY_HPP

#include "ClientList.hpp"
#include "../games/ISGame.hpp"

namespace RC::Server {
	enum LobbyState {
		WAITING_FOR_PLAYER,
		CHOOSING_GAME,
		IN_GAME
	};

	class Lobby {
	public:
		uint32_t id;
		ClientList players;
		LobbyState state = WAITING_FOR_PLAYER;
		std::unique_ptr<ISGame> game = nullptr;
		std::thread game_thread;

		Lobby(uint32_t id, const std::shared_ptr<Client> &owner);
		Network::NLobby toNLobby();
		Lobby &join(const std::shared_ptr<Client> &client);
		void leave(Client &client);
		void startGame(Network::GameID game_ID);
		std::vector<Network::NPlayer> getNPlayers();
		bool operator==(const Lobby &other) const noexcept {return this->id == other.id;};
		bool isEmpty() const noexcept {return this->players.isEmpty();};
	};
}


#endif //RCONSOLE_LOBBY_HPP
