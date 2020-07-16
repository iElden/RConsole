#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <3ds.h>
#include <functional>
#include <string>

class Keyboard {

public:
	void spawn(char *buf, int len, int lr, std::function<void (std::string	)> f = nullptr);

private:
	SwkbdState _kb;
};

#endif // KEYBOARD_HPP