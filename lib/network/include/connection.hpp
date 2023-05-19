#pragma once

#include "socket.hpp"
#include "socket_address.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string_view>
#include <sys/socket.h>

class Connection {
  public:
    Connection() = default;

    explicit Connection(Socket &&socket) : socket_(std::move(socket)) {}

    explicit Connection(const SocketAddress &address) { connect(address); }

    void connect(const SocketAddress &address);

    void write_str(const std::string &str);

    std::string read(size_t bytes);

    std::string readUntil(const std::string &terminator);

    void close() { socket_.close(); }

  private:
    Socket socket_;
};