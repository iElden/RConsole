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

			auto c = this->clients.createClient(listener);

			c->attach("packet_received", [this, c](const Network::Packet &packet){
				this->onPacketReceived(c, packet);
			});
		}
	}

	void Main::onCreateLobby(const std::shared_ptr<Client> &client)
	{
		std::shared_ptr<Lobby> lobby = lobbies.createLobby(client);
		client->connection.sendLobbyCreated(lobby->toNLobby());
	}

	void Main::onDeleteLobby(const std::shared_ptr<Client> &client)
	{
		Lobby &lobby = this->lobbies.getLobbyByClient(*client);
		this->lobbies.delLobby(lobby);
		client->connection.sendOk();
	}

	void Main::onJoinLobby(const std::shared_ptr<Client> &client, uint32_t lobby_id)
	{
		Lobby &lobby = this->lobbies.getLobbyById(lobby_id);
		lobby.join(client);
		client->connection.sendLobbyJoined(lobby.getNPlayers());
	}

	void Main::onLeaveLobby(const std::shared_ptr<Client> &client)
	{
		Lobby &lobby = this->lobbies.getLobbyByClient(*client);
		lobby.leave(*client);
		client->connection.sendOk();
	}

	void Main::onLobbyListRequest(const std::shared_ptr<Client> &client)
	{
		client->connection.sendLobbyList(this->lobbies.getNLobbies());
	}

	void Main::onLobbyStateRequest(const std::shared_ptr<Client> &client)
	{
		Lobby &lobby = this->lobbies.getLobbyByClient(*client);
		client->connection.sendLobbyState(lobby.getNPlayers());
	}

	void Main::onPacketReceived(const std::shared_ptr<Client> &client, const Network::Packet &packet)
	{
		assert(client.use_count() >= 2);

		using namespace RC::Network;

		printf("opcode %d\n", packet.header.code);
		try {
			switch (packet.header.code) {
			case Opcode::GOODBYE:
				client->connection.disconnect();
				break;
			case JOIN_LOBBY:
				this->onJoinLobby(client, packet.joinLobby.id);
				break;
			case CREATE_LOBBY:
				this->onCreateLobby(client);
				break;
			case DELETE_LOBBY:
				this->onDeleteLobby(client);
				break;
			case LEAVE_LOBBY:
				this->onLeaveLobby(client);
				break;
			case LOBBY_LIST_REQUEST:
				this->onLobbyListRequest(client);
				break;
			case LOBBY_STATE_REQUEST:
				this->onLobbyStateRequest(client);
				break;
			default:
				throw InvalidOpcodeException(packet.header.code);
			}
		}
		catch (ServerException &exc) {
			client->connection.sendError(exc.what());
		}
	}
}