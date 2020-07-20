//
// Created by Gegel85 on 20/07/2020.
//

#ifndef RCONSOLE_CGAME_HPP
#define RCONSOLE_CGAME_HPP


#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <TGUI/Text.hpp>
#include "../ICGame.hpp"
#include "PongPackets.hpp"

namespace RC::Pong
{
	class CGame : public ICGame {
	private:
		std::pair<int, int> _score;
		Network::Ball _ball;
		Network::Racket _p1;
		Network::Racket _p2;
		sf::CircleShape _circle;
		sf::RectangleShape _rect;
		tgui::Text _text;

	public:
		CGame();
		void render(sf::RenderTarget &target) override;
		void onPacketReceived(const void *packet, size_t size) override;
	};
}


#endif //RCONSOLE_CGAME_HPP
