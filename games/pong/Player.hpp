/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Player.hpp
*/
#ifndef RCONSOLE_PLAYER_HPP
#define RCONSOLE_PLAYER_HPP


#include "../../server/Client.hpp"
#include "PongPackets.hpp"

namespace RC::Pong {
	class Player {
	private:
		std::shared_ptr<Server::Client> _client;

	public:
		Player(const std::shared_ptr<Server::Client> &client_ptr);
		void send_update(const Network::PacketUpdate &packetUpdate);
	};
}


#endif //RCONSOLE_PLAYER_HPP