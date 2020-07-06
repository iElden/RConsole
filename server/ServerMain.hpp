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
#include <thread>
#include "ClientList.hpp"
#include "LobbyList.hpp"

namespace RC::Server {
	class Main {
		ClientList clients;
		LobbyList lobbies;
		std::thread _waitingThread;

		void onCreateLobby(Client &client);
		void onJoinLobby(Client &client, uint32_t lobby_id);
		void onDeleteLobby(Client &client);
		void onLeaveLobby(Client &client);
		void onLobbyListRequest(Client &client);
		void onLobbyStateRequest(Client &client);
		void onSetReady(Client &client);
		void onChooseGame(Client &client);

		std::shared_ptr<Client> &getClientPtr(const Client &client);

	public:
		void run(unsigned short port);

		void onPacketReceived(Client &client, const Network::Packet &packet);
	};
}


#endif //RCONSOLE_SERVERMAIN_HPP
