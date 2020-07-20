/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ISGame.hpp
*/

#ifndef RCONSOLE_ISGAME_HPP
#define RCONSOLE_ISGAME_HPP

#include "../server/Client.hpp"

namespace RC {
	class ISGame {
		virtual void run() = 0;
	};
}

#endif //RCONSOLE_ISGAME_HPP
