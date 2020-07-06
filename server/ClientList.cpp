/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientList.cpp
*/

#include "ClientList.hpp"

RC::Server::ClientList::ClientList(const std::shared_ptr<RC::Server::Client> &lobby_owner)
{
	this->_clients.push_back(lobby_owner);
}

std::shared_ptr<RC::Server::Client> &RC::Server::ClientList::createClient(sf::TcpListener &listener)
{
	this->max_id++;
	return this->_clients.emplace_back(new Client(this->max_id, listener));
}
