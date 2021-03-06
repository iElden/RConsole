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
		int lobby_count = 0;
		std::vector<std::shared_ptr<Lobby>> _lobbies = {};
	public:

		std::shared_ptr<Lobby> getLobbyPtrByClient(Client &client) noexcept;
		Lobby &getLobbyByClient(Client &client);
		std::shared_ptr<Lobby> getLobbyPtrById(uint32_t) noexcept;
		Lobby &getLobbyById(uint32_t id);
		Lobby &createLobby(const std::shared_ptr<Client> &owner);
		void delLobby(Lobby &lobby);
		std::vector<Network::NLobby> getNLobbies() const;
		// Overload Iterator
		auto begin() {return this->_lobbies.begin();};
		auto end() {return this->_lobbies.end();};
	};
};


#endif //RCONSOLE_LOBBYLIST_HPP
