/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** Client.hpp
*/
#ifndef RCONSOLE_CLIENT_HPP
#define RCONSOLE_CLIENT_HPP

#include <thread>
#include "../network/ClientConnection.hpp"

namespace RC::Server
{
	class Main;

	class Client {
	private:
		bool _destroyed = false;
		std::thread _thread;
		std::map<std::string, std::vector<std::function<void (const Network::Packet &)>>> _handlers;

		void _run();

	public:
		uint32_t id;
		uint32_t ping = 0;
		std::string username = "client-" + std::to_string(this->id);
		Network::ClientConnection connection;

		Client(sf::TcpListener &listener);
		~Client();
		void getUser(const std::string &username, const std::string &password);
		operator Network::NPlayer() const;
		void attach(std::string signalName, const std::function<void (const Network::Packet &)> &handler);
		bool emit(std::string signalName, const Network::Packet &packet);
		bool operator==(const Client &other) const noexcept {return this->id == other.id;};
	};
}


#endif //RCONSOLE_CLIENT_HPP
