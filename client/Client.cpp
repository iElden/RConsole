//
// Created by Gegel85 on 30/06/2020.
//

#include "Client.hpp"

#include <memory>
#include "Utils.hpp"
#include "Controller/Exceptions.hpp"
#include "../games/pong/CGame.hpp"

namespace RC::Client
{
	Client::Client() :
		_window({640, 480}, "RConsole"),
		_gui(this->_window)
	{
		this->_window.setFramerateLimit(60);
		this->_hookNetworkHandler();

		for (int i = 0; i < 3; i++) {
			this->_buffers[i].loadFromFile("gui/menu" + std::to_string(i + 1) + ".ogg");
			this->_menu[i].setBuffer(this->_buffers[i]);
			this->_menu[i].setLoop(true);
		}

		this->_loadMainPage();
	}

	Client::~Client()
	{
		this->_controllerConnecting = false;
		if (this->_client.isConnected())
			this->_client.disconnect();
		if (this->_clientThread.joinable())
			this->_clientThread.join();
		if (this->_controllerConnectThread.joinable())
			this->_controllerConnectThread.join();
	}

	int Client::run()
	{
		try {
			while (this->_window.isOpen()) {
				this->_window.clear();

				this->_waiting = true;
				this->_handleWindowEvents();

				if (this->_currentGame) {
					this->_currentGame->render(this->_window);
					this->_window.setView(this->_currentGame->getView());
				} else {
					this->_gui.draw();
					this->_window.setView(sf::View{sf::FloatRect(0, 0, this->_window.getSize().x, this->_window.getSize().y)});
				}

				do
					this->_waiting = false;
				while (!this->_done);

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
			case sf::Event::Resized:
				return this->_gui.setView(sf::View{sf::FloatRect(0, 0, event.size.width, event.size.height)});
			default:
				return;
			}
		}
	}

	void Client::_setController(unsigned short port)
	{
		this->_controller = std::make_unique<Controller::MobileController>(port);
	}

	void Client::_disconnectController()
	{
		if (this->_controller)
			try {
				this->_controller->disconnect();
			} catch (...) {}
		this->_controller.reset(nullptr);
	}

	void Client::_openControllerConnectWindow(tgui::Button::Ptr remoteConnect, tgui::Button::Ptr remoteDisconnect)
	{
		auto window = Utils::openWindowWithFocus(this->_gui, 150, 120);

		window->setTitle("Connect to server");
		window->loadWidgetsFromFile("gui/controller.gui");

		auto port = window->get<tgui::EditBox>("Port");
		auto error = window->get<tgui::TextBox>("Error");
		auto ok = window->get<tgui::Button>("OK");
		auto cancel = window->get<tgui::Button>("Cancel");

		ok->onClick.connect([this, window, remoteConnect, remoteDisconnect, port, error]{
			try {
				unsigned long p = std::stoul(port->getText().toAnsiString());

				if (p > UINT16_MAX)
					throw std::invalid_argument("");
				this->_addController(p, [window, remoteConnect, remoteDisconnect]{
					remoteConnect->setVisible(false);
					remoteDisconnect->setVisible(true);
					window->close();
				}, [error](const std::string &str){
					error->setText(str);
				});
			} catch (std::invalid_argument &e) {
				error->setText("Port is invalid");
				return;
			} catch (std::out_of_range &e) {
				error->setText("Port is invalid");
				return;
			} catch (std::exception &e) {
				error->setText(e.what());
				return;
			}
		});
		cancel->onClick.connect([window]{
			window->close();
		});
	}

	void Client::_addController(unsigned short port, const std::function<void()> &onConnected, const std::function<void(const std::string &msg)> &onError)
	{
		auto window = Utils::openWindowWithFocus(this->_gui, 180, 60);

		window->setTitle("Connecting to server");
		window->loadWidgetsFromFile("gui/connecting.gui");

		auto cancel = window->get<tgui::Button>("Cancel");

		this->_controllerConnecting = false;
		if (this->_controllerConnectThread.joinable())
			this->_controllerConnectThread.join();
		this->_controllerConnecting = true;
		this->_controllerConnectThread = std::thread([this, port, window, onConnected, onError]{
			while (this->_controllerConnecting) {
				try {
					this->_setController(port);
					onConnected();
					this->_controllerConnecting = false;
					window->close();
				} catch (Controller::TimeOutException &) {
				} catch (std::exception &e) {
					onError(e.what());
					window->close();
				}
			}
			window->close();
		});
		window->onClose.connect([this]{
			this->_controllerConnecting = false;
		});
		cancel->onClick.connect([window]{
			window->close();
		});
	}

	void Client::_hookNetworkHandler()
	{
		this->_client.attach(Network::opcodeToString.at(Network::LOBBY_LIST), [this](const Network::Packet &packet){
			this->_handleLobbyListPacket(packet);
		});
		this->_client.attach(Network::opcodeToString.at(Network::LOBBY_CREATED), [this](const Network::Packet &packet){
			this->_handleLobbyCreatedPacket(packet);
		});
		this->_client.attach(Network::opcodeToString.at(Network::LOBBY_DELETED), [this](const Network::Packet &packet){
			this->_loadMainPage();
		});
		this->_client.attach(Network::opcodeToString.at(Network::ERROR), [this](const Network::Packet &packet){
			this->_handleErrorPacket(packet.error.error, "Server error");
		});
		this->_client.attach(Network::opcodeToString.at(Network::KICKED), [this](const Network::Packet &packet){
			this->_loadMainPage();
			this->_handleErrorPacket(packet.kicked.reason, "Kicked");
		});
		this->_client.attach(Network::opcodeToString.at(Network::GAME_START), [this](const Network::Packet &packet){
			this->_startGame(packet.gameStart.gameSelected);
		});
		this->_client.attach(Network::opcodeToString.at(Network::LOBBY_JOINED), [this](const Network::Packet &packet){
			this->_loadLobbyPage();
		});
		this->_client.attach(Network::opcodeToString.at(Network::PLAYER_LEFT), [this](const Network::Packet &packet){
			if (this->_client.isInLobby())
				this->_loadLobbyPage();
		});
		this->_client.attach(Network::opcodeToString.at(Network::PLAYER_JOINED), [this](const Network::Packet &packet){
			if (this->_client.isInLobby())
				this->_loadLobbyPage();
		});
		this->_client.attach("clientError", [this](const Network::Packet &packet){
			this->_loadMainPage();
			this->_handleErrorPacket(packet.error.error, "Client error");
		});
		this->_client.attach("lobbyLeft", [this](const Network::Packet &){
			this->_loadMainPage();
		});
	}

	void Client::_loadLobbyPage()
	{
		this->_lockGUI();

		this->_switchMusic(2);
		this->_gui.loadWidgetsFromFile("gui/lobbies.gui");

		auto leave = this->_gui.get<tgui::Button>("Leave");
		auto panel = this->_gui.get<tgui::Panel>("Players");
		auto &lobby = this->_client.getLobby();

		for (int i = 0; i < lobby.players.size(); i++) {
			auto &player = lobby.players[i];
			auto button = tgui::Button::create(player.getName());

			button->setSize({"&.w - 30 - picture1.x - picture1.w", 40});
			button->setEnabled(false);
			button->setPosition({"picture1.x + picture1.w + 10", 10 + 50 * i});
			panel->add(button);
		}
		leave->onClick.connect([this]{
			this->_client.leaveLobby();
		});

		bool visible = lobby.players[0] == this->_client.getPlayer();

		for (const auto &name : this->_gui.getWidgetNames()) {
			if (name.substring(0, strlen("play")) != "play")
				continue;

			auto play = this->_gui.get<tgui::Button>(name);
			auto id = std::stol(name.substring(strlen("play")).toAnsiString());

			play->setVisible(visible);
			play->onClick.connect([this, id] {
				this->_client.startGame(static_cast<Network::GameID>(id));
			});
		}
		this->_currentGame.reset();
		this->_unlockGUI();
	}

	void Client::_loadMainPage()
	{
		this->_lockGUI();

		this->_gui.loadWidgetsFromFile("gui/mainPage.gui");

		auto connect = this->_gui.get<tgui::Button>("Connect");
		auto disconnect = this->_gui.get<tgui::Button>("Disconnect");
		auto remoteConnect = this->_gui.get<tgui::Button>("RemoteConnect");
		auto remoteDisconnect = this->_gui.get<tgui::Button>("RemoteDisconnect");
		auto newLobby = this->_gui.get<tgui::Button>("NewLobby");
		auto openConnectWindow = [this, connect, disconnect, newLobby]{
			auto window = Utils::openWindowWithFocus(this->_gui, 200, 170);

			this->_switchMusic(0);
			this->_gui.get<tgui::Panel>("Lobbies")->removeAllWidgets();
			connect->setVisible(true);
			disconnect->setVisible(false);
			newLobby->setVisible(false);
			window->setTitle("Connect to server");
			window->loadWidgetsFromFile("gui/connect.gui");

			auto ip = window->get<tgui::EditBox>("IP");
			auto port = window->get<tgui::EditBox>("Port");
			auto error = window->get<tgui::TextBox>("Error");
			auto password = window->get<tgui::EditBox>("Password");
			auto username = window->get<tgui::EditBox>("Username");
			auto ok = window->get<tgui::Button>("OK");
			auto cancel = window->get<tgui::Button>("Cancel");

			ok->onClick.connect([this, window, connect, disconnect, ip, port, password, username, error, newLobby]{
				try {
					unsigned long p = std::stoul(port->getText().toAnsiString());

					if (p > UINT16_MAX)
						throw std::invalid_argument("");
					this->_client.connect(ip->getText(), p, username->getText(), password->getText());
					if (this->_clientThread.joinable())
						this->_clientThread.join();
					this->_clientThread = std::thread([this]{
						this->_client.run();
					});
					connect->setVisible(false);
					newLobby->setVisible(true);
					disconnect->setVisible(true);
					this->_switchMusic(1);
				} catch (std::invalid_argument &e) {
					error->setText("Port is invalid");
					return;
				} catch (std::out_of_range &e) {
					error->setText("Port is invalid");
					return;
				} catch (std::exception &e) {
					error->setText(e.what());
					return;
				}
				window->close();
			});
			cancel->onClick.connect([window]{
				window->close();
			});
		};

		connect->onClick.connect(openConnectWindow);
		disconnect->onClick.connect([this, connect, openConnectWindow]{
			this->_client.disconnect();
			openConnectWindow();
		});
		remoteConnect->onClick.connect([this, remoteConnect, remoteDisconnect]{
			this->_openControllerConnectWindow(remoteConnect, remoteDisconnect);
		});
		remoteDisconnect->onClick.connect([this, remoteConnect, remoteDisconnect, newLobby]{
			remoteConnect->setVisible(true);
			remoteDisconnect->setVisible(false);
			this->_disconnectController();
		});
		newLobby->onClick.connect([this]{
			this->_client.makeLobby();
		});

		if (!this->_client.isConnected())
			openConnectWindow();
		else
			this->_switchMusic(1);

		newLobby->setVisible(this->_client.isConnected());
		connect->setVisible(!this->_client.isConnected());
		disconnect->setVisible(this->_client.isConnected());

		auto panel = this->_gui.get<tgui::Panel>("Lobbies");
		const auto &lobbies = this->_client.getLobbyList();

		panel->removeAllWidgets();
		for (size_t i = 0; i < lobbies.size(); i++) {
			auto &lobby = lobbies[i];
			auto button = tgui::Button::create("Lobby " + std::to_string(lobby.id));

			button->setSize({"&.w - 20", 40});
			button->setPosition({10, 10 + 50 * i});
			button->onClick.connect([this, lobby]{
				this->_client.joinLobby(lobby.id);
			});
			panel->add(button);
		}
		this->_currentGame.reset();
		this->_unlockGUI();
	}

	void Client::_handleLobbyListPacket(const Network::Packet &packet)
	{
		this->_lockGUI();

		auto panel = this->_gui.get<tgui::Panel>("Lobbies");

		panel->removeAllWidgets();
		for (int i = 0; i < packet.lobbyList.nbLobby; i++) {
			auto &lobby = packet.lobbyList.lobbies[i];
			auto button = tgui::Button::create("Lobby " + std::to_string(lobby.id));

			button->setSize({"&.w - 20", 40});
			button->setPosition({10, 10 + 50 * i});
			button->onClick.connect([this, lobby]{
				this->_client.joinLobby(lobby.id);
			});
			panel->add(button);
		}

		this->_unlockGUI();
	}

	void Client::_handleLobbyCreatedPacket(const Network::Packet &packet)
	{
		this->_lockGUI();

		auto &lobby = packet.lobbyCreated.lobby;
		auto button = tgui::Button::create("Lobby " + std::to_string(lobby.id));
		auto panel = this->_gui.get<tgui::Panel>("Lobbies");

		button->setSize({460, 40});
		button->setPosition({10, 10 + 50 * panel->getWidgetNames().size()});
		button->onClick.connect([this, lobby]{
			this->_client.joinLobby(lobby.id);
		});
		panel->add(button);

		this->_unlockGUI();
	}

	void Client::_handleErrorPacket(const std::string &error, const std::string &title)
	{
		this->_lockGUI();

		auto win = Utils::msgWin(title, error, MB_ICONERROR);
		auto panel = tgui::Panel::create({"100%", "100%"});

		panel->getRenderer()->setBackgroundColor({0, 0, 0, 175});
		this->_gui.add(panel);

		win->setPosition("(&.w - w) / 2", "(&.h - h) / 2");
		win->setFocused(true);

		const bool tabUsageEnabled = this->_gui.isTabKeyUsageEnabled();
		auto closeWindow = [this, win, panel, tabUsageEnabled]{
			this->_gui.remove(win);
			this->_gui.remove(panel);
			this->_gui.setTabKeyUsageEnabled(tabUsageEnabled);
		};

		panel->connect("Clicked", closeWindow);
		win->connect({"Closed", "EscapeKeyPressed"}, closeWindow);
		this->_gui.add(win);

		this->_unlockGUI();
	}

	void Client::_startGame(Network::GameID id)
	{
		switch (id) {
		case Network::GAME_ID_PONG:
			this->_currentGame.reset(new Pong::CGame());
			break;
		default:
			throw InvalidGameException(id);
		}

		for (int i = 0; i < 3; i++)
			this->_menu[i].stop();

		auto view = this->_currentGame->getView();

		this->_window.setView(view);
		this->_client.attach(Network::opcodeToString.at(Network::GAME_EVENT), [this](const Network::Packet &packet){
			if (!this->_controller)
				this->_currentGame->onPacketReceived(packet.gameEvent.gameData, packet.header.dataSize - sizeof(packet.header.code), this->_client, this->_defaultController);
			else
				this->_currentGame->onPacketReceived(packet.gameEvent.gameData, packet.header.dataSize - sizeof(packet.header.code), this->_client, *this->_controller);
		});
	}

	void Client::_lockGUI()
	{
		this->_done = false;
		while (this->_waiting);
	}

	void Client::_unlockGUI()
	{
		this->_done = true;
	}

	void Client::_switchMusic(unsigned char newMusic)
	{
		unsigned currentMusic = 0;

		while (currentMusic < 3 && this->_menu[currentMusic].getStatus() != sf::Music::Playing)
			currentMusic++;

		if (currentMusic == 3) {
			this->_menu[newMusic].setLoop(true);
			this->_menu[newMusic].play();
			return;
		}

		if (currentMusic == newMusic)
			return;

		this->_menu[newMusic].setPlayingOffset(this->_menu[currentMusic].getPlayingOffset());
		this->_menu[newMusic].setLoop(true);
		this->_menu[newMusic].play();
		this->_menu[currentMusic].stop();
		this->_menu[currentMusic].setLoop(true);
	}
}