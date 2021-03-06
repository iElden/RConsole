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

namespace RC::Network
{
	class Connection {
	protected:
		sf::TcpSocket _sock;
		static void _checkSFMLStatus(const sf::Socket::Status &status);

	public:
		void disconnect();

		void sendOk();
		void sendError(const std::string &error);
		void sendGoodbye();
		void sendGameEvent(const void *data, uint64_t size);

		template <typename T>
		void sendData(const T &data) {
			this->sendRawData(&data, sizeof(T));
		};
		void sendRawData(const void *packet, size_t size);
		void receiveNextPacket(Packet *&buffer);
	};
}


#endif //RCONSOLE_CONNECTION_HPP
