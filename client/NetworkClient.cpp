/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** NetworkClient.cpp
*/

#include "NetworkClient.hpp"
#include "Exceptions.hpp"

namespace RC::Client
{
	bool NetworkClient::isInLobby() const noexcept
	{
		return this->_myLobby.has_value();
	}

	const RClientLobby &NetworkClient::getLobby() const
	{
		if (!this->isInLobby())
			throw NotInLobbyException();
		return *this->_myLobby;
	}

	void NetworkClient::connect(const std::string &ip, unsigned int port, const std::string &username, const std::string &password)
	{
		Network::Packet packet;

		ServerConnection::connect(ip, port);
		this->sendHello(username, password);
		this->receiveNextPacket(packet);

		if (packet.header.code == Network::ERROR)
			throw ConnectException(packet.error.error);
		else if (packet.header.code != Network::OLLEH)
			throw ConnectException("Handshake with the server failed");

		this->_me.emplace(RPlayer(packet.olleh.id, username));
	}

	const RPlayer &NetworkClient::getPlayer() const
	{
		return *this->_me;
	}

	void NetworkClient::disconnect()
	{
		try {
			this->sendGoodbye();
		} catch (...) {}
		ServerConnection::disconnect();
	}
}