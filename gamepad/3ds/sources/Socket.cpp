#include "Socket.hpp"
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>

static u32 *SOC_buffer = nullptr;

Socket::Socket()
{
    SOC_buffer = (u32 *)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

    if (SOC_buffer == nullptr) {
        this->_error.set("memalign: failed to allocate");
        return;
    }

    if (socInit(SOC_buffer, SOC_BUFFERSIZE) != 0) {
        this->_error.set("socInit failed.");
        return;
    }

    this->_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (this->_socket == -1) {
        this->_error.set("invalid socket");
        return;
    }

    this->_addrSize = sizeof(this->_addr);

    if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0) {
        this->_error.set("fcntl: " + std::string(strerror(errno)));
        return;
    }
}

Socket::~Socket()
{
    close(this->_socket);
}

bool Socket::set(std::string ip, std::string port)
{
    if (inet_pton(AF_INET, ip.c_str(), &(this->_addr.sin_addr)) == 0) {
        this->_error.set("invalid id address >> {" + ip + "}");
        return false;
    }
    if (port.empty()) {
        this->_error.set("port is empty");
        return false;
    }

    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(std::stoi(port));
    return true;
}

bool Socket::send(char *msg)
{
    auto ret = sendto(this->_socket, msg, 16, 0, (struct sockaddr *) &this->_addr, this->_addrSize);

    if (ret < 0) {
        this->_error.set(strerror(errno));
        return false;
    }
    return true;
}

char *Socket::receive()
{
    static char msg[2] = {0};

    auto ret = recvfrom(this->_socket, msg, 2, 0, (struct sockaddr *) &this->_addr, &this->_addrSize);

    if (ret < 0) {
        this->_error.set(strerror(errno));
        return nullptr;
    }
    return msg;
}

bool Socket::error()
{
    return this->_error.hasOccured();
}

std::string Socket::getLastError()
{
    return this->_error.getLast();
}

bool Socket::isConnected()
{
    return this->_connected;
}

void Socket::setConnected(bool state)
{
    this->_connected = state;
}
