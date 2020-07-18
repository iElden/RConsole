#ifndef TEXT_HPP
#define TEXT_HPP

#include <array>
#include <string>
#include "Vector2.hpp"

class Text {
public:
    Text(std::string text, Vector2<int> position, std::array<VINT, 2> touchArea = {VINT{-1, -1}, VINT{-1, -1}}, int color = 37);
    void clear();
    void display(std::string s = "");

    void setText(std::string text);
    void setColor(int color);

    bool isTouched(Vector2<int> touchPosition);

    const char *c_str() const;
    std::string str() const;

private:
    std::string _text;
    Vector2<int> _position;
    std::array<VINT, 2> _touchArea;
    int _color;
};

#endif // TEXT_HPP
