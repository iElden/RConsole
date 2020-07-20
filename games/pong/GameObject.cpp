/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** GameObject.cpp
*/

#include <cmath>
#include "GameObject.hpp"
#include "SGame.hpp"

#define COLLIDED1D(b, a, c) ((a>b)^(b>c))
#define COLLIDED2D(r, op, np) (COLLIDED1D(r.pos.x, op.x, np.x) && COLLIDED1D(op.y, r.pos.y, r.pos.y+r.size))

RC::Pong::Vector2::operator Network::Position() const noexcept
{
	return RC::Pong::Network::Position{this->x, this->y};
}

RC::Pong::Racket::operator Network::Racket() const noexcept
{
	return RC::Pong::Network::Racket{this->pos, this->size, this->direction1D, this->speed};
}

RC::Pong::Ball::operator Network::Ball() const noexcept
{
	return RC::Pong::Network::Ball{this->pos, this->speed, this->angle};
}

void RC::Pong::Ball::update(const Racket &r1, const Racket &r2) noexcept
{
	Vector2 new_pos;
	if (this->angle < 180)
		new_pos = {this->pos.x + static_cast<int>(speed), this->pos.y};
	else
		new_pos = {this->pos.x - static_cast<int>(speed), this->pos.y};

	if (COLLIDED2D(r2, this->pos, new_pos)) { // ball collide with r1
		this->angle = std::fmod((this->angle + 180), 360);
	}
	if (COLLIDED2D(r2, this->pos, new_pos)) {  // ball collide with r2
		this->angle = std::fmod((this->angle + 180), 360);
	}
}

void RC::Pong::Racket::move(RC::Pong::Direction1D new_dir)
{
	this->direction1D = new_dir;

	switch (new_dir) {
	case NONE:
		return;
	case UP:
		this->pos.y -= this->speed;
		break;
	case DOWN:
		this->pos.y += this->speed;
		break;
	}
	this->pos.y = std::max(std::min(this->pos.y, PONG_MAX_Y - this->size), 0);
}
