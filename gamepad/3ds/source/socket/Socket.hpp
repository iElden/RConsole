#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <3ds.h>
#include <string>
#include "../info/Info.hpp"

#define ERROR info.setStatus(NOT_CONNECTED); this->_error = true; return;

class Socket {
	public:
		Socket();
		~Socket();


		void tryConnection(Info &info);

		void waitForResponse();

		void send(std::string data);

	private:
		int _socket;
		int _port;
		struct hostent *hostinfo = nullptr;
		struct sockaddr_in _sa;
		std::string _msg;
		bool _error = false;
};

#endif // SOCKET_HPP