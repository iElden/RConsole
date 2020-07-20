#include "Touch.hpp"

void Touch::scan()
{
    hidScanInput();
    hidTouchRead(&this->_position);
}

Vector2<int> Touch::getPosition()
{
    return Vector2<int>{this->_position.px, this->_position.py};
}
