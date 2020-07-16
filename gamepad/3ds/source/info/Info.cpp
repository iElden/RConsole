#include "Info.hpp"
#include "../Output/Output.h"
#include <cstdio>

Info::Info()
{}

void Info::display()
{
	Output::printAt(this->_ip, 10, 24);
	Output::printAt(this->_id, 20, 24);

	Output::printAt(message.at(this->_status), 25, 24);
}

static std::string paddString(std::string s)
{
	std::string paddedString = s.substr(0, 15);

	while (paddedString.size() < 15)
		paddedString.append(" ");
	return paddedString;
}

void Info::setId(std::string s)
{
	this->_id = paddString(s);
}

void Info::setIp(std::string s)
{
	this->_ip = paddString(s);
}

void Info::setStatus(STATUS s)
{
	this->_status = s;
}

std::string Info::getIp()
{
	return this->_ip;
}

std::string Info::getId()
{
	return this->_id;
}