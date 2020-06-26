/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Connection.cpp
*/

#include "Connection.hpp"
#include "Exceptions.hpp"

namespace RC::Network
{
	void Connection::host(unsigned short port)
	{

	}

	void Connection::connect(const std::string &ip, unsigned short port)
	{

	}

	void Connection::sendHello(const std::string &username, const std::string &password)
	{

	}

	void Connection::sendOlleh(uint32_t id)
	{

	}

	void Connection::sendPing(uint32_t lastPing)
	{

	}

	void Connection::sendPong()
	{

	}

	void Connection::sendGoodbye()
	{

	}

	void Connection::sendKicked(const std::string &reason)
	{

	}

	void Connection::sendJoinLobby(uint32_t id)
	{

	}

	void Connection::sendCreateLobby(const NLobby &lobby)
	{

	}

	void Connection::sendDeleteLobby()
	{

	}

	void Connection::sendLeaveLobby()
	{

	}

	void Connection::sendLobbyJoined(const std::vector<NPlayer> &players)
	{

	}

	void Connection::sendLobbyCreated(const NLobby &lobby)
	{

	}

	void Connection::sendLobbyDeleted(const NLobby &lobby)
	{

	}

	void Connection::sendPlayerJoined(const NPlayer &player)
	{

	}

	void Connection::sendOk()
	{

	}

	void Connection::sendError(const std::string &error)
	{

	}

	void Connection::sendLobbyListRequest()
	{

	}

	void Connection::sendLobbyList(const std::vector<NLobby> &lobbies)
	{

	}

	void Connection::sendLobbyStateRequest()
	{

	}

	void Connection::sendLobbyState(const std::vector<NPlayer> &players)
	{

	}

	void Connection::sendPlayerReady()
	{

	}

	void Connection::sendSetReady()
	{

	}

	void Connection::sendChooseGame(uint32_t id)
	{

	}

	void Connection::sendGameStart(uint32_t gameSelected)
	{

	}

	void Connection::sendGameEvent(const void *data, uint64_t size)
	{

	}

	void Connection::receiveNextPacket(Packet &buffer)
	{
		size_t readSize;
		uint64_t packetSize;

		Connection::_checkSFMLStatus(
			this->_sock.receive(
				&packetSize,
				sizeof(packetSize),
				readSize
			)
		);
	}

	void Connection::sendRawData(const void *data, size_t size)
	{
		Connection::_checkSFMLStatus(this->_sock.send(data, size));
	}

	void Connection::_checkSFMLStatus(const sf::Socket::Status &status)
	{
		switch (status) {
		case sf::Socket::Status::Disconnected:
			throw DisconnectedException();
		case sf::Socket::Status::Error:
			throw SocketErrorException();
		case sf::Socket::Status::NotReady:
			throw SocketNotReadyException();
		case sf::Socket::Status::Partial:
		case sf::Socket::Status::Done:
			break;
		}
	}
}

/*void RC::Network::Connection::sendRawPacket(const void *packet, unsigned int size)
{

}*/
