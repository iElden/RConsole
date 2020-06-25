/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RClient.cpp
*/
#include "RClient.hpp"

bool RC::RClient::isInLobby() const noexcept
{
	return this->myLobby.has_value();
}

const RC::RClientLobby &RC::RClient::getLobby() const
{
	return *this->myLobby;
}
