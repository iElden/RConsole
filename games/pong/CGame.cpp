//
// Created by Gegel85 on 20/07/2020.
//

#include <TGUI/Global.hpp>
#include "CGame.hpp"
#include "../../client/Exceptions.hpp"

namespace RC::Pong
{
	CGame::CGame()
	{
		this->_rect.setOutlineThickness(0);
		this->_rect.setFillColor(sf::Color::White);

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
		this->_rect.setPosition(this->_p1.pos.x, this->_p1.pos.y);
		target.draw(this->_rect);

		this->_rect.setSize({
			20,
			static_cast<float>(this->_p2.size)
		});
		this->_rect.setPosition(this->_p2.pos.x, this->_p2.pos.y);
		target.draw(this->_rect);

		this->_circle.setPosition(this->_ball.pos.x, this->_ball.pos.y);
		target.draw(this->_circle);

		this->_text.setPosition(20, 20);
		this->_text.setString(
			"P1 pos: (" + std::to_string(this->_p1.pos.x) + ", " + std::to_string(this->_p1.pos.y) + ")\n" +
			"P2 pos: (" + std::to_string(this->_p2.pos.x) + ", " + std::to_string(this->_p2.pos.y) + ")\n" +
			"Ball pos: (" + std::to_string(this->_ball.pos.x) + ", " + std::to_string(this->_ball.pos.y) + ")\n" +
			"Ball speed: " + std::to_string(this->_ball.speed) + "\n" +
			"Ball angle: " + std::to_string(this->_ball.angle) + "\n" +
			"Score: " + std::to_string(this->_score.first) + "/" + std::to_string(this->_score.second)
		);
		this->_text.draw(target, {});
	}

	void CGame::onPacketReceived(const void *packet, size_t size)
	{
		const auto &pack = *reinterpret_cast<const Network::Packet *>(packet);

		switch (pack.opcode) {
		case Network::GAME_UPDATE:
			this->_ball = pack.update.ball;
			this->_p1 = pack.update.racket1;
			this->_p2 = pack.update.racket2;
			break;
		case Network::SCORE:
			this->_score.first = pack.score.score_1;
			this->_score.second = pack.score.score_2;
			break;
		case Network::INPUT_REQ:
		default:
			throw Client::InvalidOpcodeException(pack.opcode);
		}
	}
}