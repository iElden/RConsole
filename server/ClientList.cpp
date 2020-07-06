/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientList.cpp
*/

#include "ClientList.hpp"
#include "exc.hpp"

RC::Server::ClientList::ClientList(const std::shared_ptr<RC::Server::Client> &lobby_owner)
{
	this->_clients.push_back(lobby_owner);
}

std::shared_ptr<RC::Server::Client> &RC::Server::ClientList::getClientPtr(const Client &client)
{
	for (std::shared_ptr<RC::Server::Client> &cl : *this) {
		if (*cl == client)
			return cl;
	}
	throw NotFound("Client not found");
}

RC::Server::Client &RC::Server::ClientList::createClient(sf::TcpListener &listener, RC::Server::Main &main)
{
	this->max_id++;
	return *this->_clients.emplace_back(new Client(this->max_id, listener, main));
}
