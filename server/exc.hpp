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
		ServerException(const std::string &&msg): _msg(msg) {};
		const char *what() const noexcept override { return this->_msg.c_str(); };
	};

	class NotFound : public ServerException {
		NotFound(const std::string &msg): ServerException(msg) {};
	};
}

#endif //RCONSOLE_EXC_HPP
