//
// Created by Gegel85 on 30/06/2020.
//

#ifndef RCONSOLE_CLIENT_HPP
#define RCONSOLE_CLIENT_HPP


#include "NetworkClient.hpp"

namespace RC
{
	class Client {
	private:
		NetworkClient _client;

	public:
		Client() = default;
	};
}


#endif //RCONSOLE_CLIENT_HPP
