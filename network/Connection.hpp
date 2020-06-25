/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Connection.hpp
*/
#ifndef RCONSOLE_CONNECTION_HPP
#define RCONSOLE_CONNECTION_HPP

namespace RC::Network {
	class Connection {
	public:
		void sendRawPacket(const void *packet, unsigned int size);
	};
}


#endif //RCONSOLE_CONNECTION_HPP
