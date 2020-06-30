/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** NetworkClient.hpp
*/

#ifndef RCONSOLE_NETWORKCLIENT_HPP
#define RCONSOLE_NETWORKCLIENT_HPP

#include <optional>
#include "RClientLobby.hpp"
#include "../network/ClientConnection.hpp"

namespace RC::Client
{
	class NetworkClient : Network::ClientConnection {
	private:
		std::optional<RPlayer> _me = std::nullopt;
		std::optional<RClientLobby> _myLobby = std::nullopt;

	public:
		void connect(const std::string &ip, unsigned port);
		bool isInLobby() const noexcept;
		const RPlayer &getPlayer() const;
		const RClientLobby &getLobby() const; // raise if not in lobby
	};
}


#endif //RCONSOLE_NETWORKCLIENT_HPP
