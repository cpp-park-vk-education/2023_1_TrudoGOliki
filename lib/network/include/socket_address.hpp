#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <stdexcept>
#include <string>

class SocketAddress {
  public:
    SocketAddress(const std::string &ip, uint16_t port, in_addr_t addr)
        : addr_{} {
        addr_.sin_family = AF_INET;
        addr_.sin_port = ::htons(port);
        addr_.sin_addr.s_addr = htonl(addr);

        /*int res = ::inet_aton(ip.c_str(), &addr_.sin_addr);
        if (res == 0) {
            using namespace std::string_literals;
            throw std::runtime_error("Cannot parse ip from string "s +
                                     std::strerror(errno));
        }*/
    }

    const ::sockaddr_in &get() const { return addr_; }

  private:
    ::sockaddr_in addr_;
};