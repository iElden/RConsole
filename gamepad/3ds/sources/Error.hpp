#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error {
public:
    void set(std::string what);

    std::string getLast();
    bool hasOccured();

private:
    std::string _what;
    bool _error = false;
};

#endif // ERROR_HPP
