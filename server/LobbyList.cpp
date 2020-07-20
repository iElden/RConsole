/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** LobbyList.cpp
*/

#include "LobbyList.hpp"
#include "exc.hpp"



namespace RC::Server {
	Lobby &LobbyList::getLobbyByClient(Client &client)
	{
		std::shared_ptr<Lobby> lobby = this->getLobbyPtrByClient(client);
		if (lobby == nullptr)
			throw NotFound("Lobby not found.");
		return *lobby;
	}

	std::shared_ptr<Lobby> LobbyList::getLobbyPtrByClient(Client &client) noexcept
	{
		for (std::shared_ptr<Lobby> &lobby : this->_lobbies) {
			for (const std::shared_ptr<Client> &cl : lobby->players) {
				if (*cl == client)
					return lobby;
			}
		}
		return nullptr;
	}

	Lobby &LobbyList::createLobby(const std::shared_ptr<Client> &owner)
	{
		std::shared_ptr<Lobby> lobby_ptr = this->getLobbyPtrByClient(*owner);
		if (lobby_ptr != nullptr) {
			throw AlreadyInLobby();
		}

		std::shared_ptr<Lobby> lobby = std::make_shared<Lobby>(this->lobby_count, owner);
		this->lobby_count++;
		this->_lobbies.push_back(lobby);
		return *lobby;
	}

	void LobbyList::delLobby(Lobby &lobby)
	{
		// kick all client in lobby
		for (const std::shared_ptr<Client> &cl : lobby.players) {
			cl->connection.sendKicked("The lobby has been deleted");
		}

		// remove lobby from vector
		this->_lobbies.erase(
			std::remove_if(
				this->_lobbies.begin(),
				this->_lobbies.end(),
				[&lobby](const std::shared_ptr<Lobby> &lb) { return lobby == *lb; }
			),
			this->_lobbies.end()
		);
	}

	std::shared_ptr<Lobby> LobbyList::getLobbyPtrById(uint32_t id) noexcept
	{
		for (std::shared_ptr<Lobby> &lobby : this->_lobbies) {
			if (lobby->id == id)
				return lobby;
		}
		return nullptr;
	}

	Lobby &LobbyList::getLobbyById(uint32_t id)
	{
		std::shared_ptr<Lobby> lobby = this->getLobbyPtrById(id);
		if (lobby == nullptr)
			throw NotFound("Lobby not found.");
		return *lobby;
	}

	std::vector<Network::NLobby> LobbyList::getNLobbies() const
	{
		std::vector<Network::NLobby> result;

		for (const std::shared_ptr<Lobby> &lobby : this->_lobbies)
			result.push_back(lobby->toNLobby());
		return result;
	}
}