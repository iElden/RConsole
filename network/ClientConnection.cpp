/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientConnection.cpp
*/

#include "ClientConnection.hpp"
#include <memory.h>

#define COPY_MESSAGE(src, dest) ::memset(dest, 0, MAX_MESSAGE), src.copy(dest, MAX_MESSAGE - 1)
