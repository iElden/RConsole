//
// Created by Gegel85 on 20/07/2020.
//

#ifndef RCONSOLE_CGAME_HPP
#define RCONSOLE_CGAME_HPP


#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <TGUI/Text.hpp>
#include <SFML/Audio/Music.hpp>
#include "../ICGame.hpp"
#include "PongPackets.hpp"

namespace RC::Pong
{
	class CGame : public ICGame {
	private:
		const Client::Lobby &_lobby;
		sf::Music _music;
		std::pair<int, int> _score;
		Network::Ball _ball{{0, 0}, 0, 0};
		Network::Racket _p1{{0, 0}, 0, 500, NONE};
		Network::Racket _p2{{0, 0}, 0, 500, NONE};
		sf::CircleShape _circle;
		sf::RectangleShape _rect;
		tgui::Text _text;
		unsigned timer1 = 950;
		unsigned timer2 = 0;

		void _sendInput(Client::Controller::IController &controller, Client::NetworkClient &client);
		void _displayMeterBar(sf::RenderTarget &target, const Network::Racket &racket, unsigned &timer, sf::Color color, sf::Vector2f pos, bool goLeft);

	public:
		CGame(const Client::Lobby &lobby);
		~CGame();
		void render(sf::RenderTarget &target) override;
		void onPacketReceived(const void *packet, size_t size, Client::NetworkClient &client, Client::Controller::IController &controller) override;
		sf::View getView() const override;
	};
}


#endif //RCONSOLE_CGAME_HPP
