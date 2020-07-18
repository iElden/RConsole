//
// Created by Gegel85 on 26/06/2020.
//

#ifndef RCONSOLE_EXCEPTIONS_HPP
#define RCONSOLE_EXCEPTIONS_HPP


#include <string>

namespace RC::Network
{
	class NetworkException : public std::exception {
	private:
		std::string _msg;

	public:
		NetworkException(const std::string &&msg): _msg(msg) {};

		const char *what() const noexcept override { return this->_msg.c_str(); };
	};

	class SocketNotReadyException : public NetworkException {
	public:
		SocketNotReadyException(): NetworkException("The socket is not ready") {};
	};

	class DisconnectedException : public NetworkException {
	public:
		DisconnectedException(): NetworkException("The socket is not connected") {};
	};

	class SocketErrorException : public NetworkException {
	public:
		SocketErrorException(): NetworkException("The socket encountered an unexpected error") {};
	};

	class InvalidPacketSizeException : public NetworkException {
	public:
		InvalidPacketSizeException(size_t expected):
		NetworkException("The packet had an invalid size. Couldn't read " + std::to_string(expected) + " bytes") {};
		InvalidPacketSizeException(size_t got, size_t expected):
		NetworkException("The packet had an invalid size. Expected " + std::to_string(expected) + " bytes but got " + std::to_string(got) + " bytes") {};
	};

	class ConnectException : public NetworkException {
	public:
		ConnectException(const std::string &ip, unsigned short port):
			NetworkException("Cannot connect to " + ip + ":" + std::to_string(port)) {};
	};
}


#endif //RCONSOLE_EXCEPTIONS_HPP
