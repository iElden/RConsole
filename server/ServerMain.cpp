/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Server.cpp
*/

#include "ServerMain.hpp"
namespace RC::Server {
	void Main::run()
	{
		puts("Hello !");
	}

	void Main::onCreateLobby(std::shared_ptr<Client> &client)
	{
		std::shared_ptr<Lobby> lobby = lobbies.createLobby(client);
		client->connection.sendLobbyCreated(lobby->toNLobby());
	}

	void Main::onDeleteLobby(std::shared_ptr<Client> &client)
	{
		Lobby &lobby = this->lobbies.getLobbyByClient(*client);
		this->lobbies.delLobby(lobby);
		client->connection.sendOk();
	}
}