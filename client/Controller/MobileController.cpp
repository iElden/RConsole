//
// Created by Gegel85 on 14/07/2020.
//

#include "MobileController.hpp"
#include "Exceptions.hpp"

namespace RC::Client::Controller
{
	MobileController::MobileController(unsigned short port)
	{
		try {
			Network::Packet packet;

			this->_remotePort = port;
			this->_remote = sf::IpAddress::Any;

			try {
				this->_checkSFMLStatus(this->_sock.bind(this->_remotePort));
			} catch (std::exception &e) {
				throw BindFailedException(this->_remotePort, e.what());
			}

			this->_sock.setBlocking(false);
			this->_connected = true;
			this->_receiveNextPacket(&packet, sizeof(packet));

			if (packet.opcode != Network::HELLO)
				throw InvalidHandshakeException("Invalid handshake received from controller");

			this->sendOlleh();
		} catch (...) {
			this->_sock.unbind();
			this->_connected = false;
			throw;
		}
	}

	void MobileController::_checkSFMLStatus(const sf::Socket::Status &status)
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

	Network::Keys MobileController::getKeys()
	{
		Network::InputPacket packet;

		this->sendInputReq(this->_vibrate);
		this->_receiveNextPacket(&packet, sizeof(packet));

		if (packet.opcode == Network::GOODBYE) {
			this->_connected = false;
			throw NotConnectedException();
		}
		if (packet.opcode != Network::INPUTS)
			throw InvalidHandshakeException("Invalid response to an INPUT_REQ");

		return packet.keys;
	}

	void MobileController::sendOlleh()
	{
		Network::Opcode packet{
			Network::OLLEH
		};

		if (!this->_connected)
			throw NotConnectedException();
		this->_checkSFMLStatus(this->_sock.send(&packet, sizeof(packet), this->_remote, this->_remotePort));
	}

	void MobileController::sendGoodbye()
	{
		Network::Opcode packet{
			Network::GOODBYE
		};

		if (!this->_connected)
			throw NotConnectedException();
		this->_checkSFMLStatus(this->_sock.send(&packet, sizeof(packet), this->_remote, this->_remotePort));
	}

	void MobileController::sendInputReq(bool vibrate)
	{
		Network::InputReqPacket packet{
			Network::INPUT_REQ,
			vibrate
		};

		if (!this->_connected)
			throw NotConnectedException();
		this->_checkSFMLStatus(this->_sock.send(&packet, sizeof(packet), this->_remote, this->_remotePort));
	}

	void MobileController::setVibrating(bool vibrate)
	{
		this->_vibrate = vibrate;
	}

	size_t MobileController::_receiveNextPacket(void *packet, size_t size)
	{
		size_t realSize;

		if (!this->_connected)
			throw NotConnectedException();
		this->_timeout.restart();
		while (this->_timeout.getElapsedTime().asSeconds() < 1) {
			try {
				this->_checkSFMLStatus(
					this->_sock.receive(packet, size, realSize, this->_remote, this->_remotePort)
				);
				return realSize;
			} catch (SocketNotReadyException &) {}
		}
		throw TimeOutException(1);
	}

	void MobileController::disconnect()
	{
		this->sendGoodbye();
		this->_connected = false;
	}
}