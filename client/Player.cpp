/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RPlayer.cpp
*/

#include "Player.hpp"

namespace RC::Client
{
	Player::Player(unsigned id, const std::string &name) :
		id(id),
		name(name),
		state(CONNECTING),
		ping(-1)
	{}

	bool Player::operator==(const Player &other) const noexcept
	{
		return this->id == other.id;
	}

	const std::string &Player::getName() const noexcept
	{
		return name;
	}

	unsigned Player::getId() const noexcept
	{
		return this->id;
	}
}