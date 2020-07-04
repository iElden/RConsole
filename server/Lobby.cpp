/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Lobby.cpp
*/

#include "Lobby.hpp"

RC::Server::Lobby::Lobby(uint32_t id, std::shared_ptr<RC::Server::Client> &owner):
	id(id),
	players(owner)
{}

RC::Network::NLobby RC::Server::Lobby::toNLobby()
{
	return Network::NLobby{this->id};
}
