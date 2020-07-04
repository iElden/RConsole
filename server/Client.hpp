/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Client.hpp
*/
#ifndef RCONSOLE_CLIENT_HPP
#define RCONSOLE_CLIENT_HPP

#include "../network/ClientConnection.hpp"

namespace RC::Server {
	class Client {
	public:
		uint32_t id;
		Network::ClientConnection connection;

		bool operator==(const Client &other) const noexcept {return this->id == other.id;};
	};
}


#endif //RCONSOLE_CLIENT_HPP
