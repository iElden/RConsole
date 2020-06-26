/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Utils.cpp
*/

#include <cstring>
#include "Utils.hpp"

namespace RC::Network::Utils
{
	void copyToBuffer(char *buffer, const std::string &str, size_t buffSize)
	{
		std::memset(buffer, 0, buffSize);
		str.copy(buffer, buffSize - 1);
	}
}