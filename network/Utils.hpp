/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Utils.hpp
*/

#ifndef RCONSOLE_UTILS_HPP
#define RCONSOLE_UTILS_HPP

#include <string>

namespace RC::Network::Utils
{
	void copyToBuffer(char *buffer, const std::string &str, size_t buffSize);
}

#endif //RCONSOLE_UTILS_HPP
