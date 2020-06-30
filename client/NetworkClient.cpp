/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** NetworkClient.cpp
*/

#include "NetworkClient.hpp"
#include "Exceptions.hpp"

namespace RC
{
	bool NetworkClient::isInLobby() const noexcept
	{
		return this->_myLobby.has_value();
	}

	const RClientLobby &NetworkClient::getLobby() const
	{
		if (!this->isInLobby())
			throw NotInLobbyException();
		return *this->_myLobby;
	}
}