//
// Created by Gegel85 on 20/07/2020.
//

#ifndef RCONSOLE_KEYBOARDCONTROLLER_HPP
#define RCONSOLE_KEYBOARDCONTROLLER_HPP


#include "IController.hpp"

namespace RC::Client::Controller
{
	class KeyboardController : public IController {
	public:
		Network::Keys getKeys() override;
	};
}


#endif //RCONSOLE_KEYBOARDCONTROLLER_HPP
