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
	void Connection::sendGoodbye()
	{
		PacketHeader packet{
			sizeof(packet) - sizeof(packet.dataSize),
			GOODBYE
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
