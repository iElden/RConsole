//
// Created by Gegel85 on 30/06/2020.
//

#ifndef RCONSOLE_CLIENT_HPP
#define RCONSOLE_CLIENT_HPP


#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <thread>
#include "NetworkClient.hpp"
#include "Controller/Controller.hpp"

namespace RC::Client
{
	class Client {
	private:
		std::thread _clientThread;
		bool _controllerConnecting;
		std::thread _controllerConnectThread;
		sf::RenderWindow _window;
		tgui::Gui _gui;
		NetworkClient _client;
		std::unique_ptr<Controller::Controller> _controller;

		void _hookNetworkHandler();
		void _handleWindowEvents();
		void _setController(unsigned short port);
		void _disconnectController();
		void _openControllerConnectWindow(tgui::Button::Ptr remoteConnect, tgui::Button::Ptr remoteDisconnect);
		void _addController(unsigned short port, const std::function<void()> &onConnected, const std::function<void(const std::string &msg)> &onError);
		void _handleLobbyListPacket(const Network::Packet &packet);

	public:
		Client();
		~Client();
		int run();
	};
}


#endif //RCONSOLE_CLIENT_HPP