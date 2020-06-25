/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** RPlayer.cpp
*/

#include "RPlayer.hpp"

RC::RPlayer::RPlayer(int id, std::string &name):
	id(id),
	name(name),
	state(CONNECTING),
	ping(-1)
{}

bool RC::RPlayer::operator==(const RPlayer &other) const noexcept
{
	return this->id == other.id;
}

const std::string &RC::RPlayer::getName() const noexcept
{
	return name;
}