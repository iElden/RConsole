#include "Keyboard.hpp"

Keyboard::Keyboard(int lr, int size)
{
    this->_size = std::min(this->_size, size);
    swkbdInit(&this->_keyboard, SWKBD_TYPE_NUMPAD, 1, this->_size);
    swkbdSetNumpadKeys(&this->_keyboard, lr, lr);
}

void Keyboard::spawn(std::function<void (std::string)> f)
{
    swkbdInputText(&this->_keyboard, buf, this->_size + 1);
    if (f != nullptr) {
        f(buf);
    }
}
