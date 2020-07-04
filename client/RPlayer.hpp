/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RPlayer.hpp
*/

#ifndef RCONSOLE_RPLAYER_HPP
#define RCONSOLE_RPLAYER_HPP

#include <string>

namespace RC::Client
{
	enum RPlayerState {
		CONNECTED,
		CONNECTING,
		DISCONNECTED,
		LAGGING
	};

	class RPlayer {
	private:
		unsigned id;
		std::string name;
	public:
		RPlayerState state;
		int ping;

		RPlayer(unsigned id, const std::string &name);
		bool operator==(const RPlayer& other) const noexcept;
		const std::string &getName() const noexcept;
	};
}

#endif //RCONSOLE_RPLAYER_HPP
