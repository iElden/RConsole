//
// Created by Gegel85 on 30/06/2020.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "Utils.hpp"
#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace RC::Client::Utils
{
	int	dispMsg(const std::string &title, const std::string &content, int variate)
	{
		auto button = tgui::Button::create("OK");
		auto text = tgui::TextBox::create();
		tgui::Gui gui;
		auto font = tgui::getGlobalFont();
		const auto startWidth = button->getSize().x + 102;
		unsigned width = startWidth;
		unsigned height = button->getSize().y + 60;
		float currentWidth = startWidth;
		auto size = text->getTextSize();

		std::cerr << title << std::endl << content << std::endl;
		for (char c : content) {
			currentWidth += font.getGlyph(c, size, false).advance;
			width = std::max(static_cast<unsigned>(currentWidth), width);
			if (c == '\n' || c == '\r')
				currentWidth = startWidth;
			if (c == '\n' || c == '\v')
				height += size;
			if (currentWidth >= 700) {
				currentWidth = startWidth;
				height += size;
			}
		}

		sf::RenderWindow win{{std::min(700U, width), std::min(220U, height)}, title, sf::Style::Titlebar | sf::Style::Close};
		auto pic = tgui::Picture::create("icons/error.png");
		sf::Event event;

		gui.setTarget(win);
		gui.add(button, "ok");
		gui.add(text);

		button->setPosition("&.w - w - 10", "&.h - h - 10");
		button->connect("Pressed", [&win]{
			win.close();
		});

		text->setText(content);
		text->setPosition(52, 10);
		text->setSize("ok.x - 62", "ok.y - 20");
		text->setReadOnly();
		text->getRenderer()->setBorderColor("transparent");
		text->getRenderer()->setBackgroundColor("transparent");

		pic->setPosition(10, 10);
		pic->setSize(32, 32);

		if (variate & MB_ICONERROR)
			gui.add(pic);

		while (win.isOpen()) {
			while (win.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					win.close();
				gui.handleEvent(event);
			}

			win.clear({230, 230, 230, 255});
			gui.draw();
			win.display();
		}

		return 0;
	}

	std::string getLastExceptionName()
	{
#ifdef __GNUG__
		int status;
		char *value;
		std::string name;

		auto val = abi::__cxa_current_exception_type();

		if (!val)
			return "No exception";

		value = abi::__cxa_demangle(val->name(), nullptr, nullptr, &status);
		name = value;
		free(value);
		return name;
#else
		return "Unknown exception";
#endif
	}
}