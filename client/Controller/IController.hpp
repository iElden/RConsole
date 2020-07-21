//
// Created by Gegel85 on 20/07/2020.
//

#ifndef RCONSOLE_ICONTROLLER_HPP
#define RCONSOLE_ICONTROLLER_HPP


#include "Packet.hpp"

namespace RC::Client::Controller
{
	class IController {
	public:
		virtual ~IController() = default;
		virtual Network::Keys getKeys() = 0;
	};
}


#endif //RCONSOLE_ICONTROLLER_HPP
