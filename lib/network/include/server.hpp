#pragma once

#include <functional>

#include "connection.hpp"
#include "file_system.hpp"
#include "socket.hpp"
#include "socket_address.hpp"

class Server {
  public:
    Server(const SocketAddress &bindAddress, std::fstream &f_stream,
           const std::string &name_main_dir);

    void Run();

  private:
    fs::FileSystem f_s_;
    Socket socket_;
};