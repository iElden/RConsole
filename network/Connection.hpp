/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Connection.hpp
*/

#ifndef RCONSOLE_CONNECTION_HPP
#define RCONSOLE_CONNECTION_HPP


#include <string>
#include <vector>
#include <SFML/Network.hpp>
#include "RPackets.hpp"

namespace RC::Network {
	class Connection {
	private:
		sf::TcpSocket _sock;

		static void _checkSFMLStatus(const sf::Socket::Status &status);
	public:
		void host(unsigned short port);
		void connect(const std::string &ip, unsigned short port);

		void sendHello(const std::string &username, const std::string &password);
		void sendOlleh(uint32_t id);
		void sendPing(uint32_t lastPing);
		void sendPong();
		void sendGoodbye();
		void sendKicked(const std::string &reason);
		void sendJoinLobby(uint32_t id);
		void sendCreateLobby(const NLobby &lobby);
		void sendDeleteLobby();
		void sendLeaveLobby();
		void sendLobbyJoined(const std::vector<NPlayer> &players);
		void sendLobbyCreated(const NLobby &lobby);
		void sendLobbyDeleted(const NLobby &lobby);
		void sendPlayerJoined(const NPlayer &player);
		void sendOk();
		void sendError(const std::string &error);
		void sendLobbyListRequest();
		void sendLobbyList(const std::vector<NLobby> &lobbies);
		void sendLobbyStateRequest();
		void sendLobbyState(const std::vector<NPlayer> &players);
		void sendPlayerReady(uint32_t id);
		void sendSetReady();
		void sendChooseGame(uint32_t id);
		void sendGameStart(uint32_t gameSelected);
		void sendGameEvent(const void *data, uint64_t size);

		template <typename T>
		void sendData(const T &data) {
			this->sendRawData(&data, sizeof(T));
		};
		void sendRawData(const void *packet, size_t size);
		void receiveNextPacket(Packet &buffer);
	};
}


#endif //RCONSOLE_CONNECTION_HPP
