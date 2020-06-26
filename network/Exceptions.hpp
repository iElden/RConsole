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
		SocketErrorException(): NetworkException("The socket encountered an unexpected error.") {};
	};
}


#endif //RCONSOLE_EXCEPTIONS_HPP
