#ifndef TOUCH_HPP
#define TOUCH_HPP

#include <3ds.h>
#include "Vector2.hpp"

class Touch
{
public:
    void scan();
    Vector2<int> getPosition();

private:
    touchPosition _position = {0, 0};
};

#endif // TOUCH_HPP
