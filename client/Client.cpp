//
// Created by Gegel85 on 30/06/2020.
//

#include "Client.hpp"

#include <memory>
#include "Utils.hpp"
#include "Controller/Exceptions.hpp"

namespace RC::Client
{
	Client::Client() :
		_window({640, 480}, "RConsole"),
		_gui(this->_window)
	{
		this->_window.setFramerateLimit(60);
		this->_loadMainPage();

		this->_hookNetworkHandler();
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

	void Client::_setController(unsigned short port)
	{
		this->_controller = std::make_unique<Controller::Controller>(port);
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
		this->_client.attach(Network::opcodeToString.at(Network::ERROR), [this](const Network::Packet &packet){
			this->_handleErrorPacket(packet);
		});
	}

	void Client::_loadMainPage()
	{
		this->_gui.loadWidgetsFromFile("gui/mainPage.gui");

		auto connect = this->_gui.get<tgui::Button>("Connect");
		auto disconnect = this->_gui.get<tgui::Button>("Disconnect");
		auto remoteConnect = this->_gui.get<tgui::Button>("RemoteConnect");
		auto remoteDisconnect = this->_gui.get<tgui::Button>("RemoteDisconnect");
		auto newLobby = this->_gui.get<tgui::Button>("NewLobby");
		auto openConnectWindow = [this, connect, disconnect, newLobby]{
			auto window = Utils::openWindowWithFocus(this->_gui, 200, 170);

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
			this->_client.sendCreateLobby();
		});

		if (!this->_client.isConnected())
			openConnectWindow();

		newLobby->setVisible(this->_client.isConnected());
		remoteConnect->setVisible(!this->_client.isConnected());
		remoteDisconnect->setVisible(this->_client.isConnected());
	}

	void Client::_handleLobbyListPacket(const Network::Packet &packet)
	{
		auto panel = this->_gui.get<tgui::Panel>("Lobbies");

		panel->removeAllWidgets();
		for (int i = 0; i < packet.lobbyList.nbLobby; i++) {
			auto &lobby = packet.lobbyList.lobbies[i];
			auto button = tgui::Button::create("Lobby " + std::to_string(lobby.id));

			button->setSize({460, 40});
			button->setPosition({10, 10 + 50 * i});
			button->onClick.connect([this, lobby]{
				this->_client.sendJoinLobby(lobby.id);
			});
			panel->add(button);
		}
	}

	void Client::_handleLobbyCreatedPacket(const Network::Packet &packet)
	{
		auto &lobby = packet.lobbyCreated.lobby;
		auto button = tgui::Button::create("Lobby " + std::to_string(lobby.id));
		auto panel = this->_gui.get<tgui::Panel>("Lobbies");

		button->setSize({460, 40});
		button->setPosition({10, 10 + 50 * panel->getWidgetNames().size()});
		button->onClick.connect([this, lobby]{
			this->_client.sendJoinLobby(lobby.id);
		});
		panel->add(button);
	}

	void Client::_handleErrorPacket(const Network::Packet &packet)
	{
		auto win = Utils::msgWin("Server error", packet.error.error, MB_ICONERROR);
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
	}
}