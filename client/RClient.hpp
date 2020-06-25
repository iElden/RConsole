/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RClient.hpp
*/
#ifndef RCONSOLE_RCLIENT_HPP
#define RCONSOLE_RCLIENT_HPP

#include <optional>
#include "RClientLobby.hpp"

namespace RC {
	class RClient {
		std::optional<RPlayer> me = std::nullopt;
		std::optional<RClientLobby> myLobby = std::nullopt;

		bool isInLobby() const noexcept;
		RClientLobby& getLobby() const; // raise if not in lobby
	};
}


#endif //RCONSOLE_RCLIENT_HPP
