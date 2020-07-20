//
// Created by Gegel85 on 20/07/2020.
//

#include <SFML/Window/Keyboard.hpp>
#include "KeyboardController.hpp"

namespace RC::Client::Controller
{
	Network::Keys KeyboardController::getKeys()
	{
		Network::Keys keys;

		keys.axisX = 0;
		keys.axisY = 0;
		keys.axisZ = 0;
		keys.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		keys.down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		keys.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		keys.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		keys.x = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		keys.y = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
		keys.a = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		keys.b = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
		return keys;
	}
}