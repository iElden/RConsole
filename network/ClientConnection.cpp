/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientConnection.cpp
*/

#include "ClientConnection.hpp"
#include "Utils.hpp"
#include <cstring>

namespace RC::Network
{
	void ClientConnection::host(unsigned short port)
	{
		sf::TcpListener listener;

		Connection::_checkSFMLStatus(listener.listen(port));
		Connection::_checkSFMLStatus(listener.accept(this->_sock));
	}

	void ClientConnection::sendOlleh(uint32_t id)
	{
		PacketOlleh packet{
			sizeof(packet) - sizeof(packet.dataSize),
			OLLEH,
			id
		};

		this->sendData(packet);
	}

	void ClientConnection::sendPing(uint32_t lastPing)
	{
		PacketPing packet{
			sizeof(packet) - sizeof(packet.dataSize),
			PING,
			lastPing
		};

		this->sendData(packet);
	}

	void ClientConnection::sendLobbyCreated(const NLobby &lobby)
	{
		PacketLobbyCreated packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LOBBY_CREATED,
			lobby
		};

		this->sendData(packet);
	}

	void ClientConnection::sendLobbyDeleted(const NLobby &lobby)
	{
		PacketLobbyDeleted packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LOBBY_DELETED,
			lobby
		};

		this->sendData(packet);
	}

	void ClientConnection::sendPlayerJoined(const NPlayer &player)
	{
		PacketPlayerJoined packet{
			sizeof(packet) - sizeof(packet.dataSize),
			PLAYER_JOINED,
			player
		};

		this->sendData(packet);
	}

	void ClientConnection::sendPlayerReady(uint32_t id)
	{
		PacketPlayerReady packet{
			sizeof(packet) - sizeof(packet.dataSize),
			PLAYER_READY,
			id
		};

		this->sendData(packet);
	}

	void ClientConnection::sendGameStart(uint32_t gameSelected)
	{
		PacketGameStart packet{
			sizeof(packet) - sizeof(packet.dataSize),
			GAME_START,
			gameSelected
		};

		this->sendData(packet);
	}

	void ClientConnection::sendKicked(const std::string &reason)
	{
		PacketKicked packet{
			sizeof(packet) - sizeof(packet.dataSize),
			KICKED,
			{}
		};

		Utils::copyToBuffer(packet.reason, reason, sizeof(packet.reason));
		this->sendData(packet);
	}

	void ClientConnection::sendLobbyJoined(const std::vector<NPlayer> &players)
	{
		size_t size = sizeof(PacketLobbyJoined) + players.size() * sizeof(NPlayer);
		char *buffer = new char[size];
		auto *packet = reinterpret_cast<PacketLobbyJoined *>(buffer);

		packet->dataSize = size - sizeof(packet->dataSize);
		packet->code = LOBBY_JOINED;
		packet->playerCount = players.size();
		std::memcpy(packet->players, players.data(), players.size() * sizeof(NPlayer));
		this->sendRawData(packet, size);
	}

	void ClientConnection::sendLobbyList(const std::vector<NLobby> &lobbies)
	{
		size_t size = sizeof(PacketLobbyList) + lobbies.size() * sizeof(NLobby);
		char *buffer = new char[size];
		auto *packet = reinterpret_cast<PacketLobbyList *>(buffer);

		packet->dataSize = size - sizeof(packet->dataSize);
		packet->code = LOBBY_STATE;
		packet->nbLobby = lobbies.size();
		std::memcpy(packet->lobbies, lobbies.data(), lobbies.size() * sizeof(NPlayer));
		this->sendRawData(packet, size);
	}

	void ClientConnection::sendLobbyState(const std::vector<NPlayer> &players)
	{
		size_t size = sizeof(PacketLobbyState) + players.size() * sizeof(NPlayer);
		char *buffer = new char[size];
		auto *packet = reinterpret_cast<PacketLobbyState *>(buffer);

		packet->dataSize = size - sizeof(packet->dataSize);
		packet->code = LOBBY_STATE;
		packet->playerCount = players.size();
		std::memcpy(packet->players, players.data(), players.size() * sizeof(NPlayer));
		this->sendRawData(packet, size);
	}
}