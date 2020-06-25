/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientConnection.hpp
*/
#ifndef RCONSOLE_CLIENTCONNECTION_HPP
#define RCONSOLE_CLIENTCONNECTION_HPP

#include "../network/RPackets.hpp"
#include "../network/Connection.hpp"
#include <string>

namespace RC {
	//! Represent a connection to a Client.
	class ClientConnection : public Network::Connection {

		template <typename T>
		void sendPacket(const T& packet) {
			this->sendRawPacket(&packet, sizeof(T));
		};
		void sendOk();
		void sendError(const std::string &message);
		void sendOlleh(unsigned int playerId);
		void sendPing(unsigned int lastPing);
		void sendGoodbye();
		void sendKicked(const std::string &reason="You have been kicked from lobby");
		void sendLobbyList(const class LobbyList &lobbyList);
		void sendLobbyState(const class Lobby &lobby);
	};
}


#endif //RCONSOLE_CLIENTCONNECTION_HPP
