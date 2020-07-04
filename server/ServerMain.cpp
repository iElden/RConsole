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

	void Main::onJoinLobby(std::shared_ptr<Client> &client, uint32_t lobby_id)
	{
		Lobby &lobby = this->lobbies.getLobbyById(lobby_id);
		lobby.join(client);
		client->connection.sendLobbyJoined(lobby.getNPlayers());
	}

	void Main::onLeaveLobby(std::shared_ptr<Client> &client)
	{
		Lobby &lobby = this->lobbies.getLobbyByClient(*client);
		lobby.leave(*client);
		client->connection.sendOk();
	}

	void Main::onLobbyListRequest(std::shared_ptr<Client> &client)
	{
		client->connection.sendLobbyList(this->lobbies.getNLobbies());
	}
}