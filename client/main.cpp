/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** main.cpp
*/

#include "Client.hpp"
#ifndef _WIN32
	#include <X11/Xlib.h>
#endif

int main()
{
#ifndef _WIN32
	XInitThreads();
#endif
	RC::Client::Client client;

	return client.run();
}