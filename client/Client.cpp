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
		this->_gui.loadWidgetsFromFile("gui/mainPage.gui");

		auto connect = this->_gui.get<tgui::Button>("Connect");
		auto disconnect = this->_gui.get<tgui::Button>("Disconnect");
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
	}

	Client::~Client()
	{
		if (this->_client.isConnected())
			this->_client.disconnect();
		if (this->_clientThread.joinable())
			this->_clientThread.join();
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
}