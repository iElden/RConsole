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
	void	msgWin(tgui::ChildWindow::Ptr gui, const std::string &title, const std::string &content, int variate, const std::function<void()> &onClose)
	{
		auto button = tgui::Button::create("OK");
		auto text = tgui::TextBox::create();
		auto font = tgui::getGlobalFont();
		auto pic = tgui::Picture::create("gui/error.png");

		gui->add(button, "ok");
		gui->add(text);

		button->setPosition("&.w - w - 10", "&.h - h - 10");
		button->connect("Pressed", [onClose]{
			onClose();
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
			gui->add(pic);
	}

	tgui::ChildWindow::Ptr	msgWin(const std::string &title, const std::string &content, int variate)
	{
		auto button = tgui::Button::create("OK");
		auto text = tgui::TextBox::create();
		auto font = tgui::getGlobalFont();
		const auto startWidth = button->getSize().x + 102;
		unsigned width = startWidth;
		unsigned height = button->getSize().y + 60;
		float currentWidth = startWidth;
		auto size = text->getTextSize();

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

		auto win = tgui::ChildWindow::create(title);
		sf::Event event;

		win->setSize({std::min(700U, width), std::min(220U, height)});
		msgWin(win, title, content, variate, [win]{ win->close(); });

		return win;
	}

	void	dispMsg(tgui::Gui &gui, const std::string &title, const std::string &content, int variate, const std::function<void()> &onClose)
	{
		auto button = tgui::Button::create("OK");
		auto text = tgui::TextBox::create();
		auto font = tgui::getGlobalFont();
		auto pic = tgui::Picture::create("gui/error.png");

		gui.add(button, "ok");
		gui.add(text);

		button->setPosition("&.w - w - 10", "&.h - h - 10");
		button->connect("Pressed", [onClose]{
			onClose();
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
	}

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
		sf::Event event;

		dispMsg(gui, title, content, variate, [&win]{ win.close(); });
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

	tgui::ChildWindow::Ptr openWindowWithFocus(tgui::Gui &gui, tgui::Layout width, tgui::Layout height)
	{
		auto panel = tgui::Panel::create({"100%", "100%"});

		panel->getRenderer()->setBackgroundColor({0, 0, 0, 175});
		gui.add(panel);

		auto window = tgui::ChildWindow::create();
		window->setSize(width, height);
		window->setPosition("(&.w - w) / 2", "(&.h - h) / 2");
		gui.add(window);

		window->setFocused(true);

		const bool tabUsageEnabled = gui.isTabKeyUsageEnabled();
		auto closeWindow = [&gui, window, panel, tabUsageEnabled]{
			gui.remove(window);
			gui.remove(panel);
			gui.setTabKeyUsageEnabled(tabUsageEnabled);
		};

		panel->connect("Clicked", closeWindow);
		window->connect({"Closed", "EscapeKeyPressed"}, closeWindow);
		return window;
	}
}