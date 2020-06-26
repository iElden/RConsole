/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientConnection.hpp
*/
#ifndef RCONSOLE_CLIENTCONNECTION_HPP
#define RCONSOLE_CLIENTCONNECTION_HPP

#include "RPackets.hpp"
#include "Connection.hpp"
#include <string>

namespace RC::Network
{
	//! Represent a connection to a Client.
	class ClientConnection : public Connection {
		void host(unsigned short port);

		void sendOlleh(uint32_t id);
		void sendPing(uint32_t lastPing);
		void sendKicked(const std::string &reason);
		void sendLobbyJoined(const std::vector<NPlayer> &players);
		void sendLobbyCreated(const NLobby &lobby);
		void sendLobbyDeleted(const NLobby &lobby);
		void sendPlayerJoined(const NPlayer &player);
		void sendLobbyList(const std::vector<NLobby> &lobbies);
		void sendLobbyState(const std::vector<NPlayer> &players);
		void sendPlayerReady();
		void sendGameStart(uint32_t gameSelected);
		void sendGameEvent(const void *data, uint64_t size);
	};
}


#endif //RCONSOLE_CLIENTCONNECTION_HPP
