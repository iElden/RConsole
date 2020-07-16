#include "Socket.hpp"
#include <algorithm>

Socket::Socket()
{
	this->_socket = socket(AF_INET, SOCK_DGRAM, 0);
	this->_error = (this->_socket == -1);
}

Socket::~Socket()
{
	close(this->_socket);
}

void Socket::tryConnection(Info & info)
{
	if (this->_socket == -1) {
		ERROR
	}

	info.setStatus(CONNECTING);

	std::string ip = info.getIp();
	std::string port = info.getId();

	ip.erase(remove(ip.begin(), ip.end(), ' '), ip.end());

	if (inet_pton(AF_INET, ip.c_str(), &(this->_sa.sin_addr)) < 0) {
		ERROR
	}
}