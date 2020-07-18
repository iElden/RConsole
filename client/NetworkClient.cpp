/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** NetworkClient.cpp
*/

#include <iostream>
#include <memory>
#include <cstring>
#include "NetworkClient.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"

namespace RC::Client
{
	NetworkClient::NetworkClient()
	{
		this->attach("Goodbye",       [this](const Network::Packet &packet){ this->_onGoodbye(packet); });
		this->attach("Ping",          [this](const Network::Packet &packet){ this->_onPing(packet); });
		this->attach("Lobby_List",    [this](const Network::Packet &packet){ this->_onLobbyList(packet); });
		this->attach("Lobby_Created", [this](const Network::Packet &packet){ this->_onLobbyCreated(packet); });
		this->attach("Lobby_Deleted", [this](const Network::Packet &packet){ this->_onLobbyDeleted(packet); });
		this->attach("Lobby_Joined",  [this](const Network::Packet &packet){ this->_onLobbyJoined(packet); });
		this->attach("Lobby_State",   [this](const Network::Packet &packet){ this->_onLobbyState(packet); });
		this->attach("Player_Ready",  [this](const Network::Packet &packet){ this->_onPlayerReady(packet); });
		this->attach("Player_Joined", [this](const Network::Packet &packet){ this->_onPlayerJoined(packet); });
	}

	bool NetworkClient::isInLobby() const noexcept
	{
		return this->_myLobby.has_value();
	}

	const Lobby &NetworkClient::getLobby() const
	{
		if (!this->isInLobby())
			throw NotInLobbyException();
		return *this->_myLobby;
	}

	void NetworkClient::connect(const std::string &ip, unsigned int port, const std::string &username, const std::string &password)
	{
		Network::Packet *packet = nullptr;

		try {
			ServerConnection::connect(ip, port);
			this->sendHello(username, password);
			this->receiveNextPacket(packet);

			if (packet->header.code == Network::ERROR)
				throw ConnectException(packet->error.error);
			else if (packet->header.code != Network::OLLEH)
				throw ConnectException("Handshake with the server failed");

			this->_me.emplace(Player(packet->olleh.id, username));
			this->_connected = true;

			this->sendLobbyListRequest();
		} catch (...) {
			delete packet;
			throw;
		}
		delete packet;
	}

	const Player &NetworkClient::getPlayer() const
	{
		return *this->_me;
	}

	void NetworkClient::disconnect()
	{
		this->_me.reset();
		this->_myLobby.reset();
		this->_lobbies.clear();
		this->_connected = false;
		Connection::disconnect();
	}

	const std::vector<Lobby> &NetworkClient::getLobbyList() const
	{
		return this->_lobbies;
	}

	void NetworkClient::run(const std::function<void(const std::string &)> &onError)
	{
		Network::Packet *packet = nullptr;

		try {
			while (this->_connected) {
				this->receiveNextPacket(packet);
				this->handlePacket(*packet, onError);
			}
		} catch (std::exception &e) {
			if (!this->_connected)
				return;

			std::string excName = Utils::getLastExceptionName();

			std::cerr << "Connection aborted." << std::endl << excName << ": " << e.what() << std::endl;
			this->disconnectWithError(e.what());
			delete packet;
			packet = reinterpret_cast<Network::Packet *>(
				new char[std::strlen(e.what()) + excName.size() + sizeof(Network::PacketHeader) + 3]
			);

			packet->header.code = Network::ERROR;
			packet->header.dataSize = std::strlen(e.what()) + sizeof(Network::PacketHeader) + 1;
			strcpy(packet->error.error, (excName + ": " + e.what()).c_str());
			this->emit("clientError", *packet);
			delete packet;
		}
		delete packet;
	}

	bool NetworkClient::isConnected() const
	{
		return this->_connected;
	}

	void NetworkClient::handlePacket(const Network::Packet &packet, const std::function<void(const std::string &)> &onError)
	{
		switch (packet.header.code) {
		case Network::ERROR:
			//if (!this->emit("error", packet))
			//	throw ServerErrorException(packet.error.error);
			//return;
			break;
		case Network::GOODBYE:
			this->disconnect();
			break;
		case Network::PING:
			this->_me->ping = packet.ping.lastPing;
			this->sendPong();
			break;
		case Network::KICKED:
		case Network::LOBBY_JOINED:
		case Network::LOBBY_CREATED:
		case Network::LOBBY_DELETED:
		case Network::PLAYER_JOINED:
		case Network::OK:
		case Network::LOBBY_LIST:
		case Network::LOBBY_STATE:
		case Network::PLAYER_READY:
		case Network::GAME_START:
		case Network::GAME_EVENT:
			break;
		default:
			throw InvalidOpcodeException(packet.header.code);
		}
		this->emit(Network::opcodeToString.at(packet.header.code), packet);
	}

	void NetworkClient::disconnectWithError(const std::string &error)
	{
		try {
			this->sendError(error);
		} catch (...) {}
		this->disconnect();
	}

	unsigned NetworkClient::attach(std::string signalName, const std::function<void(const Network::Packet &)> &handler)
	{
		std::transform(
			signalName.begin(),
			signalName.end(),
			signalName.begin(),
			[](unsigned char c){
				return std::tolower(c);
			}
		);

		auto &vec = this->_handlers[signalName];
		size_t i = 0;

		for (; i < vec.size() && vec[i]; i++);
		if (i == vec.size())
			vec.push_back(handler);
		else
			vec[i] = handler;
		return i;
	}

	void NetworkClient::detach(std::string signalName, unsigned int id)
	{
		std::transform(
			signalName.begin(),
			signalName.end(),
			signalName.begin(),
			[](unsigned char c){
				return std::tolower(c);
			}
		);

		this->_handlers.at(signalName).at(id) = nullptr;
	}

	bool NetworkClient::emit(std::string signalName, const Network::Packet &packet)
	{
		std::transform(
			signalName.begin(),
			signalName.end(),
			signalName.begin(),
			[](unsigned char c){
				return std::tolower(c);
			}
		);

		for (auto &handler : this->_handlers[signalName])
			if (handler)
				handler(packet);
		return this->_handlers[signalName].empty();
	}

	void NetworkClient::_onGoodbye(const Network::Packet &packet)
	{
		NetworkClient::disconnect();
	}

	void NetworkClient::_onPing(const Network::Packet &packet)
	{

	}

	void NetworkClient::_onLobbyList(const Network::Packet &packet)
	{
		this->_lobbies.clear();
		this->_lobbies.reserve(packet.lobbyList.nbLobby);

		for (unsigned i = 0; i < packet.lobbyList.nbLobby; i++) {
			auto &lobby = this->_lobbies.emplace_back();

			lobby.id = packet.lobbyList.lobbies[i].id;
			this->sendLobbyStateRequest(lobby.id);
		}
	}

	void NetworkClient::_onLobbyCreated(const Network::Packet &packet)
	{
		auto &lobby = this->_lobbies.emplace_back();

		lobby.id = packet.lobbyCreated.lobby.id;
		this->sendLobbyStateRequest(lobby.id);
	}

	void NetworkClient::_onLobbyDeleted(const Network::Packet &packet)
	{

	}

	void NetworkClient::_onLobbyJoined(const Network::Packet &packet)
	{
		this->_myLobby = Lobby{packet.lobbyJoined.lobby.id};
		for (int i = 0; i < packet.lobbyJoined.playerCount; i++)
			this->_myLobby->addPlayer(packet.lobbyJoined.players[i].id, packet.lobbyJoined.players[i].username);
	}

	void NetworkClient::_onLobbyState(const Network::Packet &packet)
	{

	}

	void NetworkClient::_onPlayerReady(const Network::Packet &packet)
	{

	}

	void NetworkClient::_onPlayerJoined(const Network::Packet &packet)
	{

	}

	void NetworkClient::makeLobby()
	{
		this->sendCreateLobby();
	}

	void NetworkClient::joinLobby(uint8_t id)
	{
		this->sendJoinLobby(id);
	}

	void NetworkClient::leaveLobby()
	{
		auto ids = std::make_shared<std::pair<unsigned, unsigned>>(0, 0);

		ids->first = this->attach(Network::opcodeToString.at(Network::OK), [this, ids](const Network::Packet &packet){
			this->emit("lobbyLeft", packet);
			this->_myLobby.reset();
			this->detach(Network::opcodeToString.at(Network::OK), ids->first);
			this->detach(Network::opcodeToString.at(Network::ERROR), ids->second);
		});
		ids->second = this->attach(Network::opcodeToString.at(Network::ERROR), [this, ids](const Network::Packet &packet){
			this->detach(Network::opcodeToString.at(Network::OK), ids->first);
			this->detach(Network::opcodeToString.at(Network::ERROR), ids->second);
		});
		this->sendLeaveLobby();
	}
}