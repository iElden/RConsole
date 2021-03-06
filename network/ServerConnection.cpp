//
// Created by Gegel85 on 26/06/2020.
//

#include "ServerConnection.hpp"
#include "../network/Utils.hpp"
#include "Exceptions.hpp"

namespace RC::Network
{
	void ServerConnection::connect(const std::string &ip, unsigned short port)
	{
		try {
			Connection::_checkSFMLStatus(this->_sock.connect(ip, port));
		} catch (...) {
			throw ConnectException(ip, port);
		}
	}

	void ServerConnection::sendPong()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			PONG
		};

		this->sendData(packet);
	}

	void ServerConnection::sendJoinLobby(uint32_t id)
	{
		PacketJoinLobby packet{
			sizeof(packet) - sizeof(packet.dataSize),
			JOIN_LOBBY,
			id
		};

		this->sendData(packet);
	}

	void ServerConnection::sendCreateLobby()
	{
		PacketCreateLobby packet{
			sizeof(packet) - sizeof(packet.dataSize),
			CREATE_LOBBY
		};

		this->sendData(packet);
	}

	void ServerConnection::sendDeleteLobby()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			DELETE_LOBBY
		};

		this->sendData(packet);
	}

	void ServerConnection::sendLeaveLobby()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LEAVE_LOBBY
		};

		this->sendData(packet);
	}

	void ServerConnection::sendLobbyListRequest()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LOBBY_LIST_REQUEST
		};

		this->sendData(packet);
	}

	void ServerConnection::sendLobbyStateRequest(uint32_t id)
	{
		PacketLobbyStateRequest packet{
			sizeof(packet) - sizeof(packet.dataSize),
			LOBBY_STATE_REQUEST,
			id
		};

		this->sendData(packet);
	}

	void ServerConnection::sendSetReady()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			SET_READY
		};

		this->sendData(packet);
	}

	void ServerConnection::sendChooseGame(GameID id)
	{
		PacketChooseGame packet{
			sizeof(packet) - sizeof(packet.dataSize),
			CHOOSE_GAME,
			id
		};

		this->sendData(packet);
	}

	void ServerConnection::sendHello(const std::string &username, const std::string &password)
	{
		PacketHello packet{
			sizeof(packet) - sizeof(packet.dataSize),
			HELLO,
			CURRENT_VERSION,
			{},
			{}
		};

		Utils::copyToBuffer(packet.username, username, sizeof(packet.username));
		Utils::copyToBuffer(packet.password, password, sizeof(packet.password));
		this->sendData(packet);
	}
}