/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientConnection.cpp
*/

#include "ClientConnection.hpp"
#include <memory.h>

#define COPY_MESSAGE(src, dest) ::memset(dest, 0, MAX_MESSAGE), src.copy(dest, MAX_MESSAGE - 1)

void RC::ClientConnection::sendOk()
{
	this->sendPacket(Network::Packet{
			Network::Opcode::OK
	});
}

void RC::ClientConnection::sendError(const std::string &message)
{
	Network::PacketError packet{
			Network::Opcode::ERROR,
	};
	COPY_MESSAGE(message, packet.error);
	this->sendPacket(packet);
}

void RC::ClientConnection::sendOlleh(unsigned int playerId)
{
	this->sendPacket(Network::PacketOlleh{
		Network::Opcode::OLLEH,
		playerId
	});
}

void RC::ClientConnection::sendPing(unsigned int lastPing)
{
	this->sendPacket(Network::PacketPing{
			Network::Opcode::PING,
			lastPing
	});
}

void RC::ClientConnection::sendGoodbye()
{
	this->sendPacket(Network::PacketPing{
			Network::Opcode::GOODBYE
	});
}

void RC::ClientConnection::sendKicked(const std::string &reason)
{
	Network::PacketKicked packet{
			Network::Opcode::KICKED,
	};
	COPY_MESSAGE(reason, packet.reason);
	this->sendPacket(packet);
}

