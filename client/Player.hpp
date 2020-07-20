/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RPlayer.hpp
*/

#ifndef RCONSOLE_PLAYER_HPP
#define RCONSOLE_PLAYER_HPP

#include <string>

namespace RC::Client
{
	enum RPlayerState {
		CONNECTED,
		CONNECTING,
		DISCONNECTED,
		LAGGING
	};

	class Player {
	private:
		unsigned id;
		std::string name;
	public:
		RPlayerState state;
		int ping;

		Player(unsigned id, const std::string &name);
		bool operator==(const Player& other) const noexcept;
		const std::string &getName() const noexcept;
		unsigned getId() const noexcept;
	};
}

#endif //RCONSOLE_PLAYER_HPP
