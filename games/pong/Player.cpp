/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Player.cpp
*/
#include "Player.hpp"

RC::Pong::Player::Player(const std::shared_ptr<RC::Server::Client> &client_ptr):
	_client(client_ptr)
{}

void RC::Pong::Player::send_update(const RC::Pong::Network::PacketUpdate &packetUpdate)
{
	this->_client->connection.sendGameEvent(&packetUpdate, sizeof(packetUpdate));
}
