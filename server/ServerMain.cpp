/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Server.cpp
*/

#include <cassert>
#include <iostream>
#include "ServerMain.hpp"
#include "exc.hpp"

namespace RC::Server
{
	void Main::run(unsigned short port)
	{
		sf::TcpListener listener;

		if (listener.listen(port) != sf::Socket::Status::Done)
			throw BindFailed(port);

		while (true) {
			std::cout << "Waiting for new client." << std::endl;
			this->clients.createClient(listener, *this);
		}
	}

	void Main::onCreateLobby(Client &client)
	{
		std::shared_ptr<Lobby> lobby = lobbies.createLobby(this->getClientPtr(client));
		client.connection.sendLobbyCreated(lobby->toNLobby());
	}

	void Main::onDeleteLobby(Client &client)
	{
		Lobby &lobby = this->lobbies.getLobbyByClient(client);
		this->lobbies.delLobby(lobby);
		client.connection.sendOk();
	}

	void Main::onJoinLobby(Client &client, uint32_t lobby_id)
	{
		Lobby &lobby = this->lobbies.getLobbyById(lobby_id);
		lobby.join(this->getClientPtr(client));
		client.connection.sendLobbyJoined(lobby.getNPlayers());
	}

	void Main::onLeaveLobby(Client &client)
	{
		Lobby &lobby = this->lobbies.getLobbyByClient(client);
		lobby.leave(client);
		client.connection.sendOk();
	}

	void Main::onLobbyListRequest(Client &client)
	{
		client.connection.sendLobbyList(this->lobbies.getNLobbies());
	}

	void Main::onPacketReceived(Client &client, const Network::Packet &packet)
	{
		//assert(client.use_count() >= 2);
	}

	std::shared_ptr<Client> &Main::getClientPtr(const Client &client)
	{
		return this->clients.getClientPtr(client);
	}
}