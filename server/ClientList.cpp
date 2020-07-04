/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientList.cpp
*/

#include "ClientList.hpp"

RC::Server::ClientList::ClientList(std::shared_ptr<RC::Server::Client> &lobby_owner)
{
	this->_clients.push_back(lobby_owner);
}