#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <3ds.h>
#include <functional>
#include <string>

#define MAX_SIZE 16

class Keyboard
{
public:
    Keyboard(int lr, int size = MAX_SIZE);

    void spawn(std::function<void (std::string)> f = nullptr);

private:
    SwkbdState _keyboard;
    char buf[MAX_SIZE - 1];
    int _size = MAX_SIZE - 1;
};

#endif // KEYBOARD_HPP
