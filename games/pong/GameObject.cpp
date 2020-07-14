/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** GameObject.cpp
*/

#include "GameObject.hpp"

RC::Pong::Vector2::operator Network::Position() const
{
	return RC::Pong::Network::Position{this->x, this->y};
}

RC::Pong::Racket::operator Network::Racket() const
{
	return RC::Pong::Network::Racket{this->x, this->size, this->direction1D, this->speed};
}

RC::Pong::Ball::operator Network::Ball() const
{
	return RC::Pong::Network::Ball{this->pos, this->speed, this->angle};
}
