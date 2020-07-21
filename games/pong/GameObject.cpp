/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** GameObject.cpp
*/

#include <cmath>
#include "GameObject.hpp"
#include "SGame.hpp"
#include "macro.hpp"

RC::Pong::Vector2::operator Network::Position() const noexcept
{
	return RC::Pong::Network::Position{this->x, this->y};
}

RC::Pong::Racket::operator Network::Racket() const noexcept
{
	return RC::Pong::Network::Racket{this->pos, this->size, static_cast<unsigned>(this->ult), this->direction1D, this->speed};
}

RC::Pong::Ball::operator Network::Ball() const noexcept
{
	return RC::Pong::Network::Ball{this->pos, this->speed, this->angle};
}

void RC::Pong::Ball::update(const Racket &r1, const Racket &r2) noexcept
{
	Vector2 new_pos;
	int aspeed = this->slowed ? (this->speed / 3) : this->speed;
	new_pos.x = this->pos.x + static_cast<int>(cos(this->angle) * aspeed);
	new_pos.y = this->pos.y + static_cast<int>(sin(this->angle) * aspeed);

	if (COLLIDED1D(0, this->pos.y, new_pos.y)) {
		this->angle = -this->angle;
		return;
	}
	if (COLLIDED1D(PONG_MAX_Y, this->pos.y, new_pos.y)) {
		this->angle = -this->angle;
		return;
	}

	if (COLLIDED2D(r1, this->pos, new_pos)) { // ball collide with r1
		this->angle = ((double) rand() / (RAND_MAX)) * 2 + 5.28318530717;
	}
	else if (COLLIDED2D(r2, this->pos, new_pos)) {  // ball collide with r2
		this->angle = ((double) rand() / (RAND_MAX)) * 2 + 2.14159265358;
	}
	else
		this->pos = new_pos;
	if (this->slowed) {
		this->slowed_timer--;
		if (!this->slowed_timer)
			this->slowed = false;
	}
	else
		this->speed += 0.01;
}

void RC::Pong::Racket::move(RC::Pong::Direction1D new_dir)
{
	this->direction1D = new_dir;

	switch (new_dir) {
	case NONE:
		return;
	case UP:
		this->pos.y -= this->speed + this->sprinting * 8;
		break;
	case DOWN:
		this->pos.y += this->speed + this->sprinting * 8;
		break;
	}
	this->pos.y = std::max(std::min(this->pos.y, PONG_MAX_Y - this->size), 0);
}

void RC::Pong::Ball::reset(float speed_)
{
	this->pos = {500, 250};
	this->speed = speed_;
	this->angle = (rand() % 2) * M_PI;
}

void RC::Pong::Ball::set_slowed()
{
	this->slowed = true;
	this->slowed_timer = TICK_PER_SECOND;
}

void RC::Pong::Racket::update()
{
	if (this->sprinting) {
		this->ult -= 2;
	}
	if (this->boosted) {
			this->ult -= 5;
		if (this->ult < 0) {
			this->ult = 0;
			this->unboost();
		}
	}
	this->ult = std::min(std::max(this->ult += 1, 0), 1000);
}

void RC::Pong::Racket::addsize(int size)
{
	this->pos.y -= size / 2;
	this->size += size;
}

void RC::Pong::Racket::boost()
{
	this->addsize(35);
	this->speed = 12.5;
	this->boosted = true;
}

void RC::Pong::Racket::unboost()
{
	this->addsize(-35);
	this->speed = 10;
	this->boosted = false;
}

void RC::Pong::Racket::strike()
{
	this->addsize(-30);
	this->striked = true;
}

void RC::Pong::Racket::unstrike()
{
	this->addsize(30);
	this->striked = false;
}