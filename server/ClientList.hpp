/*
** EPITECH PROJECT, 2020
** RConsole
** File description:
** ClientList.hpp
*/
#ifndef RCONSOLE_CLIENTLIST_HPP
#define RCONSOLE_CLIENTLIST_HPP

#include <vector>
#include <memory>
#include "Client.hpp"

namespace RC::Server
{
	class ClientList {
		std::vector<std::shared_ptr<Client>> _clients = {};
		int max_id = 0;
	public:
		ClientList() = default;
		ClientList(const std::shared_ptr<Client> &lobby_owner);

		std::shared_ptr<Client> &createClient(sf::TcpListener &listener);
		// Overload Iterator
		auto begin() {return this->_clients.begin();};
		auto end() {return this->_clients.end();};
	};
};


#endif //RCONSOLE_CLIENTLIST_HPP
