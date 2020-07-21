/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** macro.hpp
*/

#ifndef RCONSOLE_MACRO_HPP
#define RCONSOLE_MACRO_HPP

#define COLLIDED1D(b, a, c) ((a<b)?(b<=c):(b>=c))
#define COLLIDED2D(r, op, np) (COLLIDED1D(r.pos.x, op.x, np.x) && COLLIDED1D(op.y, r.pos.y, r.pos.y+r.size))
#define TICK_PER_SECOND 50
#define PONG_MAX_Y 500
#define PONG_MAX_X 1000

#endif //RCONSOLE_MACRO_HPP
