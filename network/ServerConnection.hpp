//
// Created by Gegel85 on 26/06/2020.
//

#ifndef RCONSOLE_SERVERCONNECTION_HPP
#define RCONSOLE_SERVERCONNECTION_HPP


#include <string>
#include "Connection.hpp"

namespace RC::Network
{
	//! Represent a connection to a Server.
	class ServerConnection : public Connection {
	public:
		void connect(const std::string &ip, unsigned short port);

		void sendHello(const std::string &username, const std::string &password);
		void sendPong();
		void sendJoinLobby(uint32_t id);
		void sendCreateLobby();
		void sendDeleteLobby();
		void sendLeaveLobby();
		void sendLobbyListRequest();
		void sendLobbyStateRequest(uint32_t id);
		void sendSetReady();
		void sendChooseGame(uint32_t id);
	};
}


#endif //RCONSOLE_SERVERCONNECTION_HPP
