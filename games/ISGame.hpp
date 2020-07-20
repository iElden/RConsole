/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ISGame.hpp
*/

#ifndef RCONSOLE_ISGAME_HPP
#define RCONSOLE_ISGAME_HPP

namespace RC
{
	class ISGame {
	public:
		virtual void run() = 0;
		virtual void onPacketReceived(const void *data, size_t size, Server::Client &client) = 0;
	};
}

#endif //RCONSOLE_ISGAME_HPP
