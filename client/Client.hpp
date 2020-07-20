//
// Created by Gegel85 on 30/06/2020.
//

#ifndef RCONSOLE_CLIENT_HPP
#define RCONSOLE_CLIENT_HPP


#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <thread>
#include "NetworkClient.hpp"
#include "Controller/MobileController.hpp"
#include "Controller/KeyboardController.hpp"
#include "../games/ICGame.hpp"

namespace RC::Client
{
	class Client {
	private:
		std::array<sf::Music, 3> _menu;
		bool _waiting = false;
		bool _done = true;
		Controller::KeyboardController _defaultController;
		std::unique_ptr<ICGame> _currentGame;
		std::thread _clientThread;
		bool _controllerConnecting;
		std::thread _controllerConnectThread;
		sf::RenderWindow _window;
		tgui::Gui _gui;
		NetworkClient _client;
		std::unique_ptr<Controller::MobileController> _controller;

		void _loadMainPage();
		void _loadLobbyPage();
		void _hookNetworkHandler();
		void _handleWindowEvents();
		void _setController(unsigned short port);
		void _disconnectController();
		void _openControllerConnectWindow(tgui::Button::Ptr remoteConnect, tgui::Button::Ptr remoteDisconnect);
		void _addController(unsigned short port, const std::function<void()> &onConnected, const std::function<void(const std::string &msg)> &onError);
		void _handleLobbyListPacket(const Network::Packet &packet);
		void _handleLobbyCreatedPacket(const Network::Packet &packet);
		void _handleErrorPacket(const std::string &packet, const std::string &title);
		void _startGame(Network::GameID id);
		void _lockGUI();
		void _unlockGUI();

	public:
		Client();
		~Client();
		int run();
	};
}


#endif //RCONSOLE_CLIENT_HPP
