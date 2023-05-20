#include "protocol.hpp"
#include "connection.hpp"
#include "file_system.hpp"

#include <iostream>

void Protocol::SendFile(std::string_view path, std::string_view ip) {
    auto manager_net = fs::ManagerFilesNet();
    manager_net.selectNewFile(fs::F::Path(path));
    Connection con{SocketAddress{std::string{ip}, 8080}};

    fs::Buffer buf = manager_net.getBuf();

    con.write(std::move(buf.buf_), buf.size_);

    // std::string response = con.read(10000);
    // std::cout << response << std::endl;
}