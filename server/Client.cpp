/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Client.cpp
*/
#include "Client.hpp"
#include "../network/Utils.hpp"

RC::Network::NPlayer RC::Server::Client::getNPlayer() const
{
	Network::NPlayer np = {this->id, this->ping};
	Network::Utils::copyToBuffer(np.username, this->username, MAX_USERNAME);
	return np;
}
