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

namespace RC::Server {
	class ClientList {
	private:
		std::vector<std::shared_ptr<Client>> _clients = {};

	public:
		ClientList() = default;
		ClientList(std::shared_ptr<Client> &lobby_owner);
		// Overload Iterator
		auto begin() {return this->_clients.begin();};
		auto end() {return this->_clients.end();};
	};
};


#endif //RCONSOLE_CLIENTLIST_HPP
