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
		Lobby &lobby = lobbies.createLobby(client);
		this->broadcastLobbyCreated(lobby.toNLobby());
		client->connection.sendLobbyJoined(lobby.toNLobby(), lobby.getNPlayers());
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
		client->connection.sendLobbyJoined(lobby.toNLobby(), lobby.getNPlayers());
	}

	void Main::onLeaveLobby(const std::shared_ptr<Client> &client)
	{
		Lobby &lobby = this->lobbies.getLobbyByClient(*client);
		lobby.leave(*client);
		if (lobby.isEmpty())
			this->lobbies.delLobby(lobby);
		client->connection.sendOk();
	}

	void Main::onLobbyListRequest(const std::shared_ptr<Client> &client)
	{
		client->connection.sendLobbyList(this->lobbies.getNLobbies());
	}

	void Main::onLobbyStateRequest(const std::shared_ptr<Client> &client, uint32_t id)
	{
		Lobby &lobby = this->lobbies.getLobbyById(id);
		client->connection.sendLobbyState(lobby.getNPlayers());
	}

	void Main::onPacketReceived(const std::shared_ptr<Client> &client, const Network::Packet &packet)
	{
		assert(client.use_count() >= 2);

		using namespace RC::Network;

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
				this->onLobbyStateRequest(client, packet.lobbyStateRequest.id);
				break;
			default:
				throw InvalidOpcodeException(packet.header.code);
			}
		}
		catch (ServerException &exc) {
			client->connection.sendError(exc.what());
		}
	}

	void Main::broadcastLobbyCreated(Network::NLobby id)
	{
		for (std::shared_ptr<Client> &cl : this->clients) {
			cl->connection.sendLobbyCreated(id);
		}
	}

	void Main::broadcastLobbyDeleted(Network::NLobby id)
	{
		for (std::shared_ptr<Client> &cl : this->clients) {
			cl->connection.sendLobbyDeleted(id);
		}
	}

	void Main::broadcastPlayerJoined(Network::NLobby lobby, Network::NPlayer player)
	{
		for (std::shared_ptr<Client> &cl : this->clients) {
			cl->connection.sendPlayerJoined(lobby, player);
		}
	}

	void Main::broadcastPlayerLeft(Network::NLobby lobby, Network::NPlayer player)
	{
		for (std::shared_ptr<Client> &cl : this->clients) {
			cl->connection.sendPlayerLeft(lobby, player);
		}
	}
}