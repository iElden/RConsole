#include <sstream>
#include <iostream>
#include "Text.hpp"
#include "Utils.hpp"

Text::Text(std::string text, Vector2<int> position, std::array<VINT, 2> touchArea, int color) :
    _text(text),
    _position(position),
    _touchArea(touchArea),
    _color(color)
{}

void Text::setColor(int color)
{
    this->clear();
    this->_color = color;
}

void Text::setText(std::string text)
{
    this->clear();
    this->_text = text;
}

bool Text::isTouched(Vector2<int> touchPosition)
{
    return touchPosition.isInside(this->_touchArea[0], this->_touchArea[1]);
}

void Text::clear()
{
    this->display(std::string(this->_text.size(), ' '));
}

void Text::display(std::string text)
{
    int row = (this->_position.x < 0 || this->_position.x > 50) ? 0 : this->_position.x;
    int column = (this->_position.y < 0 || this->_position.y > 50) ? 0 : this->_position.y;
    std::stringstream ss;

    ss << "\x1b[" << row << ";" << column << "H";
    ss << "\x1b[" << this->_color << "m";
    ss << (text.empty() ? this->_text : text);
    ss << "\x1b[" << 37 << "m";
    std::cout << ss.str() << std::endl;
}

const char *Text::c_str() const
{
    return this->_text.c_str();
}

std::string Text::str() const
{
    return this->_text;
}
