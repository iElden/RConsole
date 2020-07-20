//
// Created by Gegel85 on 20/07/2020.
//

#ifndef RCONSOLE_ICGAME_HPP
#define RCONSOLE_ICGAME_HPP


#include <SFML/Graphics/RenderTarget.hpp>

namespace RC
{
	class ICGame {
	public:
		virtual void render(sf::RenderTarget &target) = 0;
		virtual void onPacketReceived(const void *packet, size_t size) = 0;
	};
}


#endif //RCONSOLE_ICGAME_HPP
