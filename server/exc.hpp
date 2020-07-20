/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** exc.hpp
*/

#ifndef RCONSOLE_EXC_HPP
#define RCONSOLE_EXC_HPP

#include <string>

namespace RC::Server {
	class ServerException : public std::exception {
	private:
		std::string _msg;

	public:
		ServerException(const std::string &msg): _msg(msg) {};
		const char *what() const noexcept override { return this->_msg.c_str(); };
	};

	class InvalidHandshake : public ServerException {
	public:
		InvalidHandshake(): ServerException("Invalid handshake.") {};
	};

	class BindFailed : public ServerException {
	public:
		BindFailed(unsigned short port): ServerException("Binding to port " + std::to_string(port) + " failed.") {};
	};

	class InvalidVersion : public ServerException {
	public:
		InvalidVersion(): ServerException("The server version doesn't match.") {};
	};

	class InvalidOpcodeException : public ServerException {
	public:
		InvalidOpcodeException(uint8_t op): ServerException("Unrecognized opcode: " + std::to_string(op)) {};
	};

	class AlreadyInLobby : public ServerException {
	public:
		AlreadyInLobby(): ServerException("You are already in a lobby, please leave first.") {};
	};

	class NotFound : public ServerException {
	public:
		NotFound(const std::string &&msg): ServerException(msg) {};
	};

	class LobbyNotFound : public NotFound {
	public:
		LobbyNotFound(): NotFound("Lobby was not Found") {};
	};

	class Forbidden : public ServerException {
	public:
		Forbidden(const std::string &&msg): ServerException(msg) {};
	};
}

#endif //RCONSOLE_EXC_HPP
