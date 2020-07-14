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
		this->_gui.loadWidgetsFromFile("gui/mainPage.gui");

		auto connect = this->_gui.get<tgui::Button>("Connect");
		auto disconnect = this->_gui.get<tgui::Button>("Disconnect");
		auto remoteConnect = this->_gui.get<tgui::Button>("RemoteConnect");
		auto remoteDisconnect = this->_gui.get<tgui::Button>("RemoteDisconnect");
		auto openConnectWindow = [this, connect, disconnect]{
			auto window = Utils::openWindowWithFocus(this->_gui, 200, 170);

			connect->setVisible(true);
			disconnect->setVisible(false);
			window->setTitle("Connect to server");
			window->loadWidgetsFromFile("gui/connect.gui");

			auto ip = window->get<tgui::EditBox>("IP");
			auto port = window->get<tgui::EditBox>("Port");
			auto error = window->get<tgui::TextBox>("Error");
			auto password = window->get<tgui::EditBox>("Password");
			auto username = window->get<tgui::EditBox>("Username");
			auto ok = window->get<tgui::Button>("OK");
			auto cancel = window->get<tgui::Button>("Cancel");

			ok->onClick.connect([this, window, connect, disconnect, ip, port, password, username, error]{
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

		openConnectWindow();
		connect->onClick.connect(openConnectWindow);
		disconnect->onClick.connect([this, connect, openConnectWindow]{
			this->_client.disconnect();
			openConnectWindow();
		});
		remoteConnect->onClick.connect([this, remoteConnect, remoteDisconnect]{
			this->_openControllerConnectWindow(remoteConnect, remoteDisconnect);
		});
		remoteDisconnect->onClick.connect([this, remoteConnect, remoteDisconnect]{
			remoteConnect->setVisible(true);
			remoteDisconnect->setVisible(false);
			this->_disconnectController();
		});
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
}