//
// Created by Gegel85 on 20/07/2020.
//

#include <TGUI/Global.hpp>
#include "CGame.hpp"
#include "../../client/Exceptions.hpp"
#include "../../network/Exceptions.hpp"

namespace RC::Pong
{
	CGame::CGame(const Client::Lobby &lobby) :
		_lobby(lobby)
	{
		this->_music.openFromFile("gui/game_merged.ogg");
		this->_music.play();
		this->_music.setLoopPoints({sf::seconds(5.477), sf::seconds(32.011)});
		this->_music.setLoop(true);

		this->_rect.setOutlineThickness(0);
		this->_rect.setFillColor(sf::Color::White);

		this->_circle.setRadius(3);
		this->_circle.setOutlineThickness(0);
		this->_circle.setFillColor(sf::Color::White);

		this->_text.setFont(tgui::getGlobalFont());
		this->_text.setColor(sf::Color::White);
		this->_text.setCharacterSize(20);
	}

	void CGame::render(sf::RenderTarget &target)
	{
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

		this->_text.setPosition(2, 2);
		this->_text.setString((this->_lobby.players.size() >= 1 ? this->_lobby.players[0].getName() : "No player") + ": " + std::to_string(this->_score.first));
		this->_text.draw(target, {});

		this->_text.setPosition(800, 2);
		this->_text.setString((this->_lobby.players.size() >= 2 ? this->_lobby.players[1].getName() : "No player") + ": " + std::to_string(this->_score.second));
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
			this->_ball.pos.x = pack.update.ball.pos.x;
			this->_ball.pos.y = pack.update.ball.pos.y;
			this->_ball.speed = pack.update.ball.speed;
			this->_ball.angle = pack.update.ball.angle;

			this->_p1.speed = pack.update.racket1.speed;
			this->_p1.direction = pack.update.racket1.direction;
			this->_p1.size = pack.update.racket1.size;
			this->_p1.pos.x = pack.update.racket1.pos.x;
			this->_p1.pos.y = pack.update.racket1.pos.y;

			this->_p2.speed = pack.update.racket2.speed;
			this->_p2.direction = pack.update.racket2.direction;
			this->_p2.size = pack.update.racket2.size;
			this->_p2.pos.x = pack.update.racket2.pos.x;
			this->_p2.pos.y = pack.update.racket2.pos.y;
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
		return sf::View{{0, 0, 1000, 500}};
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