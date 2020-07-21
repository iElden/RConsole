//
// Created by Gegel85 on 20/07/2020.
//

#include <TGUI/Global.hpp>
#include "CGame.hpp"
#include "../../client/Exceptions.hpp"
#include "../../network/Exceptions.hpp"

#include <iostream>
namespace RC::Pong
{
	CGame::CGame(const Client::Lobby &lobby) :
		_lobby(lobby)
	{
		this->_music.openFromFile("gui/game_merged.ogg");
		this->_music.play();
		this->_music.setLoopPoints({sf::seconds(5.477), sf::seconds(32.011)});
		this->_music.setLoop(true);

		this->_circle.setRadius(3);
		this->_circle.setOutlineThickness(0);
		this->_circle.setFillColor(sf::Color::White);

		this->_text.setFont(tgui::getGlobalFont());
		this->_text.setCharacterSize(20);
	}

	void CGame::render(sf::RenderTarget &target)
	{
		this->_rect.setOutlineThickness(0);
		this->_rect.setPosition(-50, -100);
		this->_rect.setSize({1100, 650});
		this->_rect.setFillColor({125, 125, 125, 255});
		target.draw(this->_rect);

		this->_rect.setPosition(0, 0);
		this->_rect.setSize({1000, 500});
		this->_rect.setFillColor(sf::Color::Black);
		target.draw(this->_rect);

		this->_rect.setFillColor(sf::Color::White);
		this->_rect.setSize({
			20,
			static_cast<float>(this->_p1.size)
		});
		this->_rect.setPosition(this->_p1.pos.x - 20, this->_p1.pos.y);
		target.draw(this->_rect);

		this->_rect.setSize({
			20,
			static_cast<float>(this->_p2.size)
		});
		this->_rect.setPosition(this->_p2.pos.x, this->_p2.pos.y);
		target.draw(this->_rect);

		this->_circle.setPosition(this->_ball.pos.x - 3, this->_ball.pos.y - 3);
		target.draw(this->_circle);

		this->_rect.setFillColor(sf::Color::Blue);
		this->_rect.setPosition(0, -28);
		this->_rect.setSize({400 * this->_p1.meter / 1000.f, 20});
		target.draw(this->_rect);

		this->_rect.setFillColor(sf::Color::Red);
		this->_rect.setPosition(1000 - 400 * this->_p2.meter / 1000.f, -28);
		this->_rect.setSize({400 * this->_p2.meter / 1000.f, 20});
		target.draw(this->_rect);

		this->_rect.setOutlineThickness(3);
		this->_rect.setFillColor(sf::Color::Transparent);
		this->_rect.setSize({400, 20});

		this->_rect.setPosition(0, -28);
		target.draw(this->_rect);

		this->_rect.setPosition(600, -28);
		target.draw(this->_rect);

		this->_text.setColor(sf::Color::Blue);
		this->_text.setPosition(2, -60);
		this->_text.setString((this->_lobby.players.size() >= 1 ? this->_lobby.players[0].getName() : "No player") + ": " + std::to_string(this->_score.first));
		this->_text.draw(target, {});

		std::string str = (this->_lobby.players.size() >= 2 ? this->_lobby.players[1].getName() : "No player") + ": " + std::to_string(this->_score.second);
		unsigned pos = 1000;

		for (char c : str)
			pos -= this->_text.getFont().getGlyph(c, 20, false).advance;
		this->_text.setColor(sf::Color::Red);
		this->_text.setPosition(pos, -60);
		this->_text.setString(str);
		this->_text.draw(target, {});
	}

	void CGame::onPacketReceived(const void *packet, size_t size, Client::NetworkClient &client, Client::Controller::IController &controller)
	{
		const auto &pack = *reinterpret_cast<const Network::Packet *>(packet);

		switch (pack.opcode) {
		case Network::GAME_UPDATE:
			if (size < sizeof(Network::PacketUpdate))
				throw ::RC::Network::InvalidPacketSizeException(size, sizeof(Network::PacketUpdate));

			this->_sendInput(controller, client);
			this->_ball = pack.update.ball;
			this->_p1 = pack.update.racket1;
			this->_p2 = pack.update.racket2;
			break;
		case Network::SCORE:
			if (size < sizeof(Network::PacketScore))
				throw ::RC::Network::InvalidPacketSizeException(size, sizeof(Network::PacketScore));

			this->_score.first = pack.score.score_1;
			this->_score.second = pack.score.score_2;
			break;
		default:
			throw Client::InvalidOpcodeException(pack.opcode);
		}
	}

	sf::View CGame::getView() const
	{
		return sf::View{{-50, -100, 1100, 650}};
	}

	void CGame::_sendInput(Client::Controller::IController &controller, Client::NetworkClient &client)
	{
		Network::PacketInput packet{
			Network::GAME_INPUT
		};

		packet.keys = controller.getKeys();
		client.sendGameEvent(packet);
	}
}