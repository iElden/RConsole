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

		void onCreateLobby(const std::shared_ptr<Client> &client);
		void onJoinLobby(const std::shared_ptr<Client> &client, uint32_t lobby_id);
		void onDeleteLobby(const std::shared_ptr<Client> &client);
		void onLeaveLobby(const std::shared_ptr<Client> &client);
		void onLobbyListRequest(const std::shared_ptr<Client> &client);
		void onLobbyStateRequest(const std::shared_ptr<Client> &client, uint32_t id);
		void onGoodbye(const std::shared_ptr<Client> &client);
		void onDisconnect(const std::shared_ptr<Client> &client);
		// void onSetReady(const std::shared_ptr<Client> &client);
		void onChooseGame(const std::shared_ptr<Client> &client, uint32_t id);
		void broadcastLobbyCreated(Network::NLobby id);
		void broadcastLobbyDeleted(Network::NLobby id);
		void broadcastPlayerJoined(Network::NLobby id, Network::NPlayer player);
		void broadcastPlayerLeft(Network::NLobby id, Network::NPlayer player);

	public:
		void run(unsigned short port);

		void onPacketReceived(const std::shared_ptr<Client> &client, const Network::Packet &packet);
	};
}


#endif //RCONSOLE_SERVERMAIN_HPP
