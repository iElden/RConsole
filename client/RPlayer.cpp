/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RPlayer.cpp
*/

#include "RPlayer.hpp"

namespace RC::Client
{
	RPlayer::RPlayer(unsigned id, const std::string &name) :
		id(id),
		name(name),
		state(CONNECTING),
		ping(-1)
	{}

	bool RPlayer::operator==(const RPlayer &other) const noexcept
	{
		return this->id == other.id;
	}

	const std::string &RPlayer::getName() const noexcept
	{
		return name;
	}
}