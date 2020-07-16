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

void RC::Server::ClientList::remove(const RC::Server::Client &client)
{
	this->_clients.erase(std::remove_if(this->_clients.begin(), this->_clients.end(), [&client](const std::shared_ptr<Client> &cl){return *cl == client;}), this->_clients.end());
}

void RC::Server::ClientList::add(const std::shared_ptr<RC::Server::Client> &client)
{
	this->_clients.push_back(client);
}
