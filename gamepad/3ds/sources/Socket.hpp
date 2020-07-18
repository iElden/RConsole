#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <3ds.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Error.hpp"

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

class Socket {
public:
    Socket();
    ~Socket();

    bool error();
    std::string getLastError();

    bool set(std::string ip, std::string port);

    bool send(char *msg);
    char *receive();

    bool isConnected();
    void setConnected(bool state);

private:
    int _socket = -1;
    struct sockaddr_in _addr;
    socklen_t _addrSize = 0;

    bool _connected = false;

    Error _error;
};

#endif // SOCKET_HPP
