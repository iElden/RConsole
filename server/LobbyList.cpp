/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** LobbyList.cpp
*/

#include "LobbyList.hpp"
#include "exc.hpp"

static std::shared_ptr<RC::Server::Lobby> null = nullptr;

RC::Server::Lobby &RC::Server::LobbyList::getLobbyByClient(RC::Server::Client &client)
{
	std::shared_ptr<Lobby> lobby = this->getLobbyPtrByClient(client);
	if (lobby == nullptr)
		throw RC::Server::NotFound("Lobby don't found");
	return *lobby;
}

std::shared_ptr<RC::Server::Lobby> RC::Server::LobbyList::getLobbyPtrByClient(RC::Server::Client &client) noexcept
{
	for (std::shared_ptr<Lobby> &lobby : this->_lobbies) {
		for (std::shared_ptr<Client> &cl : lobby->players) {
			if (*cl == client)
				return lobby;
		}
	}
	return nullptr;
}

std::shared_ptr<RC::Server::Lobby> RC::Server::LobbyList::createLobby(std::shared_ptr<Client> &owner)
{
	std::shared_ptr<Lobby> lobby_ptr = this->getLobbyPtrByClient(*owner);
	if (lobby_ptr != nullptr) {
		throw AlreadyInLobby();
	}

	std::shared_ptr<Lobby> lobby = std::make_shared<Lobby>(this->lobby_count, owner);
	this->lobby_count++;
	this->_lobbies.push_back(lobby);
	return lobby;
}

void RC::Server::LobbyList::delLobby(RC::Server::Lobby &lobby)
{
	// kick all client in lobby
	for (std::shared_ptr<Client> &cl : lobby.players) {
		cl->connection.sendKicked("The lobby has been deleted");
	}

	// remove lobby from vector
	this->_lobbies.erase(std::remove_if(this->_lobbies.begin(), this->_lobbies.end(),
			                            [lobby](const std::shared_ptr<Lobby> &lb){return lobby == *lb;}),
						 this->_lobbies.end());
}
