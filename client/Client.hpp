//
// Created by Gegel85 on 30/06/2020.
//

#ifndef RCONSOLE_CLIENT_HPP
#define RCONSOLE_CLIENT_HPP


#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <thread>
#include "NetworkClient.hpp"

namespace RC::Client
{
	class Client {
	private:
		std::thread _clientThread;
		sf::RenderWindow _window;
		tgui::Gui _gui;
		NetworkClient _client;

		void _handleWindowEvents();

	public:
		Client();
		~Client();
		int run();
	};
}


#endif //RCONSOLE_CLIENT_HPP
