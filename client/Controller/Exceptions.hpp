//
// Created by Gegel85 on 30/06/2020.
//

#ifndef RCONSOLE_CONTROLLER_EXCEPTIONS_HPP
#define RCONSOLE_CONTROLLER_EXCEPTIONS_HPP


#include <string>
#include "../Exceptions.hpp"

namespace RC::Client::Controller
{
	class ControllerException : public ClientException {
	public:
		ControllerException(const std::string &&msg): ClientException(static_cast<const std::string &&>(msg)) {};
	};

	class InvalidHandshakeException : public ControllerException {
	public:
		InvalidHandshakeException(const std::string &&msg): ControllerException(static_cast<const std::string &&>(msg)) {};
	};

	class SocketNotReadyException : public ControllerException {
	public:
		SocketNotReadyException(): ControllerException("The socket is not ready") {};
	};

	class DisconnectedException : public ControllerException {
	public:
		DisconnectedException(): ControllerException("The socket is not connected") {};
	};

	class SocketErrorException : public ControllerException {
	public:
		SocketErrorException(): ControllerException("The socket encountered an unexpected error") {};
	};

	class BindFailedException : public ControllerException {
	public:
		BindFailedException(unsigned short port, const std::string &msg): ControllerException("Binding on port " + std::to_string(port) + " UDP failed: " + msg) {};
	};

	class TimeOutException : public ControllerException {
	public:
		TimeOutException(size_t time): ControllerException("Connection timed out after " + std::to_string(time) + " second(s)") {};
	};

	class NotConnectedException : public ControllerException {
	public:
		NotConnectedException(): ControllerException("The controller is not connected.") {};
	};
}


#endif //RCONSOLE_CONTROLLER_EXCEPTIONS_HPP
