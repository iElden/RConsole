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

		this->_displayMeterBar(target, this->_p1, this->timer1, sf::Color::Blue, {0, -32}, false);
		this->_displayMeterBar(target, this->_p2, this->timer2, sf::Color::Red, {1000, -32}, true);

		this->_rect.setOutlineThickness(3);
		this->_rect.setFillColor(sf::Color::Transparent);
		this->_rect.setSize({400, 20});

		this->_rect.setPosition(0, -32);
		target.draw(this->_rect);

		this->_rect.setPosition(600, -32);
		target.draw(this->_rect);

		this->_text.setColor(sf::Color::Blue);
		this->_text.setPosition(2, -64);
		this->_text.setString((this->_lobby.players.size() >= 1 ? this->_lobby.players[0].getName() : "No player") + ": " + std::to_string(this->_score.first));
		this->_text.draw(target, {});

		std::string str = (this->_lobby.players.size() >= 2 ? this->_lobby.players[1].getName() : "No player") + ": " + std::to_string(this->_score.second);
		unsigned pos = 1000;

		for (char c : str)
			pos -= this->_text.getFont().getGlyph(c, 20, false).advance;
		this->_text.setColor(sf::Color::Red);
		this->_text.setPosition(pos, -64);
		this->_text.setString(str);
		this->_text.draw(target, {});

		this->timer1 = (this->timer1 + 1) % 1000;
		this->timer2 = (this->timer2 + 1) % 950;
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

	void CGame::_displayMeterBar(sf::RenderTarget &target, const Network::Racket &racket, unsigned int &timer, sf::Color color, sf::Vector2f pos, bool goLeft)
	{
		unsigned char value = 0;

		if (timer < 50)
			value = timer * 4;
		else if (timer < 100)
			value = 200 - ((timer - 50) * 4);

		color.r = std::max(color.r, value);
		color.g = std::max(color.g, value);
		color.b = std::max(color.b, value);

		this->_rect.setFillColor(color);
		if (goLeft)
			this->_rect.setPosition(pos.x - 400 * racket.meter / 1000.f, pos.y);
		else
			this->_rect.setPosition(pos);
		this->_rect.setSize({400 * racket.meter / 1000.f, 20});
		target.draw(this->_rect);

		if (timer < 500 || timer > 550)
			return;

		this->_rect.setFillColor({
			static_cast<sf::Uint8>(200 + color.r * 50 / 255),
			static_cast<sf::Uint8>(205 + color.g * 50 / 255),
			static_cast<sf::Uint8>(205 + color.b * 50 / 255),
			255
		});
		this->_rect.setSize({10, 30});
		this->_rect.setOrigin(5, 15);
		this->_rect.setRotation(goLeft ? 22.5 : -22.5);
		if (goLeft)
			this->_rect.setPosition(pos.x - (timer - 500) * 8 + 5, pos.y + 15);
		else
			this->_rect.setPosition(pos.x + (timer - 500) * 8 + 5, pos.y + 15);
		target.draw(this->_rect);

		this->_rect.setOrigin(0, 0);
		this->_rect.setRotation(0);

		this->_rect.setFillColor({125, 125, 125, 255});
		this->_rect.setSize({410 - 400 * racket.meter / 1000.f, 20});
		if (goLeft)
			this->_rect.setPosition(pos.x - 410, pos.y);
		else
			this->_rect.setPosition(pos.x + 400 * racket.meter / 1000.f, pos.y);
		target.draw(this->_rect);

		this->_rect.setSize({10, 20});
		if (goLeft)
			this->_rect.setPosition(pos.x, pos.y);
		else
			this->_rect.setPosition(pos.x - 10, pos.y);
		target.draw(this->_rect);

		this->_rect.setSize({410, 12});
		if (goLeft)
			this->_rect.setPosition(pos.x - 400, pos.y + 20);
		else
			this->_rect.setPosition(pos.x, pos.y + 20);
		target.draw(this->_rect);
	}
}