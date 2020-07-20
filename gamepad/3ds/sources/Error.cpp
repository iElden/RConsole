#include "Error.hpp"

void Error::set(std::string what)
{
    this->_what = what;
    this->_error = !what.empty();
}

std::string Error::getLast()
{
    return this->_what;
}

bool Error::hasOccured()
{
    return this->_error;
}
