/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Server.hpp
*/
#ifndef RCONSOLE_SERVERMAIN_HPP
#define RCONSOLE_SERVERMAIN_HPP

#include <vector>
#include <memory>
#include "ClientList.hpp"
#include "LobbyList.hpp"

namespace RC::Server {
	class Main {
		ClientList clients;
		LobbyList lobbies;

		void onCreateLobby(std::shared_ptr<Client> &client);
		void onJoinLobby(std::shared_ptr<Client> &client, uint32_t lobby_id);
		void onDeleteLobby(std::shared_ptr<Client> &client);
		void onLeaveLobby(std::shared_ptr<Client> &client);
		void onLobbyListRequest(std::shared_ptr<Client> &client);
		void onLobbyStateRequest(std::shared_ptr<Client> &client);
		void onSetReady(std::shared_ptr<Client> &client);
		void onChooseGame(std::shared_ptr<Client> &client);
	public:
		void run();
	};
}


#endif //RCONSOLE_SERVERMAIN_HPP
