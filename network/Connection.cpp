/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Connection.cpp
*/

#include <cstring>
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
		PacketOlleh packet{
			sizeof(packet) - sizeof(packet.dataSize),
			OLLEH,
			id
		};

		this->sendData(packet);
	}

	void Connection::sendPing(uint32_t lastPing)
	{
		PacketPing packet{
			sizeof(packet) - sizeof(packet.dataSize),
			PING,
			lastPing
		};

		this->sendData(packet);
	}

	void Connection::sendPong()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			PONG
		};

		this->sendData(packet);
	}

	void Connection::sendGoodbye()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			GOODBYE
		};

		this->sendData(packet);
	}

	void Connection::sendKicked(const std::string &reason)
	{

	}

	void Connection::sendJoinLobby(uint32_t id)
	{
		PacketJoinLobby packet{
			sizeof(packet) - sizeof(packet.dataSize),
			JOIN_LOBBY,
			id
		};

		this->sendData(packet);
	}

	void Connection::sendCreateLobby(const NLobby &lobby)
	{
		PacketCreateLobby packet{
			sizeof(packet) - sizeof(packet.dataSize),
			CREATE_LOBBY,
			lobby
		};

		this->sendData(packet);
	}

	void Connection::sendDeleteLobby()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			DELETE_LOBBY
		};

		this->sendData(packet);
	}

	void Connection::sendLeaveLobby()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LEAVE_LOBBY
		};

		this->sendData(packet);
	}

	void Connection::sendLobbyJoined(const std::vector<NPlayer> &players)
	{

	}

	void Connection::sendLobbyCreated(const NLobby &lobby)
	{
		PacketLobbyCreated packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LOBBY_CREATED,
			lobby
		};

		this->sendData(packet);
	}

	void Connection::sendLobbyDeleted(const NLobby &lobby)
	{
		PacketLobbyDeleted packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LOBBY_DELETED,
			lobby
		};

		this->sendData(packet);
	}

	void Connection::sendPlayerJoined(const NPlayer &player)
	{
		PacketPlayerJoined packet{
			sizeof(packet) - sizeof(packet.dataSize),
			PLAYER_JOINED,
			player
		};

		this->sendData(packet);
	}

	void Connection::sendOk()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			OK
		};

		this->sendData(packet);
	}

	void Connection::sendError(const std::string &error)
	{

	}

	void Connection::sendLobbyListRequest()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LOBBY_LIST_REQUEST
		};

		this->sendData(packet);
	}

	void Connection::sendLobbyList(const std::vector<NLobby> &lobbies)
	{

	}

	void Connection::sendLobbyStateRequest()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LOBBY_STATE_REQUEST
		};

		this->sendData(packet);
	}

	void Connection::sendLobbyState(const std::vector<NPlayer> &players)
	{

	}

	void Connection::sendPlayerReady(uint32_t id)
	{
		PacketPlayerReady packet{
			sizeof(packet) - sizeof(packet.dataSize),
			PLAYER_READY,
			id
		};

		this->sendData(packet);
	}

	void Connection::sendSetReady()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			SET_READY
		};

		this->sendData(packet);
	}

	void Connection::sendChooseGame(uint32_t id)
	{
		PacketChooseGame packet{
			sizeof(packet) - sizeof(packet.dataSize),
			GAME_START,
			id
		};

		this->sendData(packet);
	}

	void Connection::sendGameStart(uint32_t gameSelected)
	{
		PacketGameStart packet{
			sizeof(packet) - sizeof(packet.dataSize),
			GAME_START,
			gameSelected
		};

		this->sendData(packet);
	}

	void Connection::sendGameEvent(const void *data, uint64_t size)
	{
		char *buffer = new char[size + sizeof(PacketGameEvent)];
		auto *packet = reinterpret_cast<PacketGameEvent *>(buffer);

		packet->code = GAME_START;
		packet->dataSize = size + sizeof(PacketGameEvent) - sizeof(packet->dataSize);
		std::memcpy(&packet->gameData, data, size);
		try {
			this->sendRawData(packet, size + sizeof(PacketGameEvent));
		} catch (...) {
			delete[] buffer;
			throw;
		}
		delete[] buffer;
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

		Connection::_checkSFMLStatus(
			this->_sock.receive(
				&buffer.header.code,
				packetSize,
				readSize
			)
		);

		if (readSize != packetSize)
			throw InvalidPacketSizeException(readSize, packetSize);

		buffer.header.dataSize = packetSize;
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
