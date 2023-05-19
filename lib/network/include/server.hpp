#pragma once

#include <functional>

#include "connection.hpp"
#include "socket.hpp"
#include "socket_address.hpp"

class Server {
  public:
    Server(const SocketAddress &bindAddress);

    void Run();

  private:
    Socket socket_;
};