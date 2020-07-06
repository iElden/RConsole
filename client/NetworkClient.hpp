/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** NetworkClient.hpp
*/

#ifndef RCONSOLE_NETWORKCLIENT_HPP
#define RCONSOLE_NETWORKCLIENT_HPP

#include <optional>
#include <functional>
#include "Lobby.hpp"
#include "../network/ClientConnection.hpp"
#include "../network/ServerConnection.hpp"

namespace RC::Client
{
	class NetworkClient : Network::ServerConnection {
	private:
		std::optional<Player> _me = std::nullopt;
		std::optional<Lobby> _myLobby = std::nullopt;
		std::vector<Lobby> _lobbies;
		bool _connected = false;
		std::map<std::string, std::vector<std::function<void (const Network::Packet &)>>> _handlers;

		void _onGoodbye(const Network::Packet &packet);
		void _onPing(const Network::Packet &packet);
		void _onLobbyList(const Network::Packet &packet);
		void _onLobbyCreated(const Network::Packet &packet);
		void _onLobbyDeleted(const Network::Packet &packet);
		void _onLobbyJoined(const Network::Packet &packet);
		void _onLobbyState(const Network::Packet &packet);
		void _onPlayerReady(const Network::Packet &packet);
		void _onPlayerJoined(const Network::Packet &packet);

	public:
		NetworkClient();

		void connect(const std::string &ip, unsigned port, const std::string &username, const std::string &password);
		void disconnectWithError(const std::string &error);
		void disconnect();
		bool isConnected() const;
		void attach(std::string signalName, const std::function<void (const Network::Packet &)> &handler);
		bool emit(std::string signalName, const Network::Packet &packet);
		bool isInLobby() const noexcept;
		const Player &getPlayer() const;
		const Lobby &getLobby() const; // raise if not in lobby
		const std::vector<Lobby> &getLobbyList() const;
		void handlePacket(const Network::Packet &packet, const std::function<void(const std::string &)> &onError);
		void run(const std::function<void(const std::string &)> &onError = {});
	};
}


#endif //RCONSOLE_NETWORKCLIENT_HPP
