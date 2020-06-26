/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientConnection.cpp
*/

#include "ClientConnection.hpp"
#include <memory.h>

#define COPY_MESSAGE(src, dest) ::memset(dest, 0, MAX_MESSAGE), src.copy(dest, MAX_MESSAGE - 1)

namespace RC::Network
{
	void ClientConnection::host(unsigned short port)
	{

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

	}

	void ClientConnection::sendLobbyJoined(const std::vector<NPlayer> &players)
	{

	}

	void ClientConnection::sendLobbyList(const std::vector<NLobby> &lobbies)
	{

	}

	void ClientConnection::sendLobbyState(const std::vector<NPlayer> &players)
	{

	}
}