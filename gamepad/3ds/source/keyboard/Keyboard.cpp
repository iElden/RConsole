#include "Keyboard.hpp"

void Keyboard::spawn(char *buf, int len, int lr, std::function<void (std::string)> f)
{
	swkbdInit(&this->_kb, SWKBD_TYPE_NUMPAD, 1, len);
	swkbdSetNumpadKeys(&this->_kb, lr, lr);
	swkbdInputText(&this->_kb, buf, len + 1);
	if (f != nullptr)
		f(std::string(buf));
}