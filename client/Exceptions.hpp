//
// Created by Gegel85 on 30/06/2020.
//

#ifndef RCONSOLE_EXCEPTIONS_HPP
#define RCONSOLE_EXCEPTIONS_HPP


#include <string>

namespace RC::Client
{
	class ClientException : public std::exception {
	private:
		std::string _msg;

	public:
		ClientException(const std::string &&msg): _msg(msg) {};

		const char *what() const noexcept override { return this->_msg.c_str(); };
	};

	class NotInLobbyException : public ClientException {
	public:
		NotInLobbyException(): ClientException("You are not in a lobby") {};
	};

	class ConnectException : public ClientException {
	public:
		ConnectException(const std::string &&reason): ClientException("Cannot connect to the server: " + reason) {};
	};

	class ServerErrorException : public ClientException {
	public:
		ServerErrorException(const std::string &&reason): ClientException("Cannot connect to the server: " + reason) {};
	};

	class InvalidOpcodeException : public ClientException {
	public:
		InvalidOpcodeException(uint8_t op): ClientException("Unrecognized opcode: " + std::to_string(op)) {};
	};
}


#endif //RCONSOLE_EXCEPTIONS_HPP
