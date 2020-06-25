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
		template <typename T>
		void sendPacket(const T& packet) {
			this->sendRawPacket(&packet, sizeof(T));
		};

		void sendRawPacket(const void *packet, unsigned int size);
	};
}


#endif //RCONSOLE_CONNECTION_HPP
