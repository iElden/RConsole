/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Game.cpp
*/
#include "Game.hpp"

RC::Pong::Network::PacketUpdate RC::Pong::Game::get_packet()
{
	return RC::Pong::Network::PacketUpdate(this->ball, this->racket1, this->racket2);
}
