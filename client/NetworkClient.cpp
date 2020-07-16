/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** NetworkClient.cpp
*/

#include <iostream>
#include "NetworkClient.hpp"
#include "Exceptions.hpp"

namespace RC::Client
{
	NetworkClient::NetworkClient()
	{
		this->attach("Goodbye",      [this](const Network::Packet &packet){ this->_onGoodbye(packet); });
		this->attach("Ping",         [this](const Network::Packet &packet){ this->_onPing(packet); });
		this->attach("LobbyList",    [this](const Network::Packet &packet){ this->_onLobbyList(packet); });
		this->attach("LobbyCreated", [this](const Network::Packet &packet){ this->_onLobbyCreated(packet); });
		this->attach("LobbyDeleted", [this](const Network::Packet &packet){ this->_onLobbyDeleted(packet); });
		this->attach("LobbyJoined",  [this](const Network::Packet &packet){ this->_onLobbyJoined(packet); });
		this->attach("LobbyState",   [this](const Network::Packet &packet){ this->_onLobbyState(packet); });
		this->attach("PlayerReady",  [this](const Network::Packet &packet){ this->_onPlayerReady(packet); });
		this->attach("PlayerJoined", [this](const Network::Packet &packet){ this->_onPlayerJoined(packet); });
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
		Network::Packet packet;

		ServerConnection::connect(ip, port);
		this->sendHello(username, password);
		this->receiveNextPacket(packet);

		if (packet.header.code == Network::ERROR)
			throw ConnectException(packet.error.error);
		else if (packet.header.code != Network::OLLEH)
			throw ConnectException("Handshake with the server failed");

		this->_me.emplace(Player(packet.olleh.id, username));
		this->_connected = true;

		this->sendLobbyListRequest();
	}

	const Player &NetworkClient::getPlayer() const
	{
		return *this->_me;
	}

	void NetworkClient::disconnect()
	{
		this->_connected = false;
		Connection::disconnect();
	}

	const std::vector<Lobby> &NetworkClient::getLobbyList() const
	{
		return this->_lobbies;
	}

	void NetworkClient::run(const std::function<void(const std::string &)> &onError)
	{
		Network::Packet packet;

		try {
			while (this->_connected) {
				this->receiveNextPacket(packet);
				this->handlePacket(packet, onError);
			}
		} catch (std::exception &e) {
			std::cerr << "Connection aborted." << std::endl << e.what() << std::endl;
			this->disconnectWithError(e.what());
		}
	}

	bool NetworkClient::isConnected() const
	{
		return this->_connected;
	}

	void NetworkClient::handlePacket(const Network::Packet &packet, const std::function<void(const std::string &)> &onError)
	{
		switch (packet.header.code) {
		case Network::ERROR:
			if (!this->emit("error", packet))
				//throw ServerErrorException(packet.error.error);
			return;
			return;
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

	void NetworkClient::attach(std::string signalName, const std::function<void(const Network::Packet &)> &handler)
	{
		std::transform(
			signalName.begin(),
			signalName.end(),
			signalName.begin(),
			[](unsigned char c){
				return std::tolower(c);
			}
		);
		this->_handlers[signalName].push_back(handler);
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

	}

	void NetworkClient::_onLobbyDeleted(const Network::Packet &packet)
	{

	}

	void NetworkClient::_onLobbyJoined(const Network::Packet &packet)
	{

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
}