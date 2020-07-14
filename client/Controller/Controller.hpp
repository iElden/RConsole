//
// Created by Gegel85 on 14/07/2020.
//

#ifndef RCONSOLE_CONTROLLER_HPP
#define RCONSOLE_CONTROLLER_HPP


#include <SFML/Network/UdpSocket.hpp>
#include <thread>
#include <SFML/System/Clock.hpp>
#include "Packet.hpp"

namespace RC::Client::Controller
{
	class Controller {
	private:
		bool _vibrate = false;
		bool _connected = false;
		std::thread _thread;
		sf::UdpSocket _sock;
		sf::IpAddress _remote;
		sf::Clock _timeout;
		unsigned short _remotePort;

		void _checkSFMLStatus(const sf::Socket::Status &status);
		size_t _receiveNextPacket(void *packet, size_t size);

	public:
		Controller(unsigned short port);
		Network::Keys getKeys();

		void sendOlleh();
		void sendGoodbye();
		void sendInputReq(bool vibrate);
		void setVibrating(bool vibrate = true);
		void disconnect();
	};
}


#endif //RCONSOLE_CONTROLLER_HPP
