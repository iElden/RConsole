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
	Client::Client(uint32_t id, sf::TcpListener &listener):
	id(id)
	{
		Network::Packet *packet = nullptr;

		while (true) {
			this->connection.host(listener);
			try {
				this->connection.receiveNextPacket(packet);

				if (packet->header.code != Network::HELLO)
					throw InvalidHandshake();

				if (packet->hello.version != CURRENT_VERSION)
					throw InvalidVersion();

				this->getUser(packet->hello.username, packet->hello.password);
				this->connection.sendOlleh(this->id);
				this->_thread = std::thread([this]{
					this->_run();
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

	void Client::getUser(const std::string &username_, const std::string &)
	{
		this->username = username_;
	}

	void Client::attach(std::string signalName, const std::function<void(const Network::Packet &)> &handler)
	{
		std::transform(
			signalName.begin(),
			signalName.end(),
			signalName.begin(),
			[](unsigned char c){
				return std::tolower(c);
			}
		);
		this->_handlers[signalName].push_back(handler);
	}

	bool Client::emit(std::string signalName, const Network::Packet &packet)
	{
		std::transform(
			signalName.begin(),
			signalName.end(),
			signalName.begin(),
			[](unsigned char c){
				return std::tolower(c);
			}
		);
		for (auto handler : this->_handlers[signalName])
			if (handler)
				handler(packet);
		return this->_handlers[signalName].empty();
	}

	void Client::_run()
	{
		Network::Packet *packet = nullptr;

		while (!this->_destroyed) {
			try {
				std::cout << "Wait for client " << this->id << " packet." << std::endl;
				this->connection.receiveNextPacket(packet);
				this->emit("packet_received", *packet);
			} catch (std::exception &e) {
				std::cerr << "Client disconnected because had error: " << e.what() << std::endl;
				try {
					this->connection.sendError(e.what());
				} catch (...) {}
				this->connection.disconnect();
				this->_destroyed = true;
				this->emit("disconnected", *packet);
			}
		}
		delete[] packet;
	}
}