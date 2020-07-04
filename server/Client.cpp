/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Client.cpp
*/
#include <iostream>
#include "Client.hpp"
#include "exc.hpp"
#include "ServerMain.hpp"
#include "../network/Utils.hpp"

namespace RC::Server
{
	Client::Client(unsigned short port, Main &main)
	{
		Network::Packet packet;

		while (true) {
			try {
				this->connection.host(port);
				this->connection.receiveNextPacket(packet);

				if (packet.header.code != Network::HELLO)
					throw InvalidHandshake();

				if (packet.hello.version != CURRENT_VERSION)
					throw InvalidVersion();

				this->getUser(packet.hello.username, packet.hello.password);
				this->connection.sendOlleh(this->id);
				this->_thread = std::thread([this, &main]{
					Network::Packet packet;

					while (!this->_destroyed) {
						try {
							std::cout << "Wait for client " << this->id << " packet." << std::endl;
							this->connection.receiveNextPacket(packet);
							main.onPacketReceived(std::shared_ptr<Client>(this), packet);
						} catch (std::exception &e) {
							std::cerr << "Client disconnected because had error: " << e.what() << std::endl;
							this->connection.sendError(e.what());
							this->connection.disconnect();
							this->_destroyed = true;
						}
					}
				});
				break;
			} catch (std::exception &e) {
				std::cerr << e.what() << std::endl;
				this->connection.sendError(e.what());
				this->connection.disconnect();
			}
		}
	}

	Client::~Client()
	{
		std::cout << "Destroying client " << this->id << std::endl;
		this->_destroyed = true;
		if (this->_thread.joinable())
			this->_thread.join();
	}

	Client::operator Network::NPlayer() const
	{
		Network::NPlayer np = {this->id, this->ping};
		Network::Utils::copyToBuffer(np.username, this->username, MAX_USERNAME);

		return np;
	}

	void Client::getUser(const std::string &username, const std::string &)
	{
		this->username = username;
		this->id = 0;
	}
}