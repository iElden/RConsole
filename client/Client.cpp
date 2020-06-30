//
// Created by Gegel85 on 30/06/2020.
//

#include "Client.hpp"
#include "Utils.hpp"

namespace RC::Client
{
	Client::Client() :
		_window({640, 480}, "RConsole"),
		_gui(this->_window)
	{
	}

	int Client::run()
	{
		try {
			while (this->_window.isOpen()) {
				this->_handleWindowEvents();
				this->_gui.draw();
				this->_window.display();
			}
			return EXIT_SUCCESS;
		} catch (std::exception &e) {
			Utils::dispMsg(Utils::getLastExceptionName(), e.what(), MB_ICONERROR);
			return EXIT_FAILURE;
		}
	}

	void Client::_handleWindowEvents()
	{
		sf::Event event;

		while (this->_window.pollEvent(event)) {
			this->_gui.handleEvent(event);

			switch (event.type) {
			case sf::Event::Closed:
				return this->_window.close();
			default:
				return;
			}
		}
	}
}