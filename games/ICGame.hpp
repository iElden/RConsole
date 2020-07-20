//
// Created by Gegel85 on 20/07/2020.
//

#ifndef RCONSOLE_ICGAME_HPP
#define RCONSOLE_ICGAME_HPP


#include <SFML/Graphics/RenderTarget.hpp>
#include "../client/Controller/MobileController.hpp"
#include "../client/NetworkClient.hpp"

namespace RC
{
	class ICGame {
	public:
		virtual void render(sf::RenderTarget &target) = 0;
		virtual void onPacketReceived(const void *packet, size_t size, Client::NetworkClient &client, Client::Controller::IController &controller) = 0;
		virtual sf::View getView() const = 0;
	};
}


#endif //RCONSOLE_ICGAME_HPP
