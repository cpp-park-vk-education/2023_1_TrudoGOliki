#include "server.hpp"
#include "connection.hpp"
#include "file_system.hpp"
#include "protocol.hpp"
#include "socket.hpp"
#include "socket_address.hpp"

#include <functional>
#include <iostream>

Server::Server(const SocketAddress &bindAddress, std::fstream &f_stream,
               const std::string &name_main_dir)
    : f_s_(fs::FileSystem(f_stream, name_main_dir)) {
    const ::sockaddr_in &sock_addr = bindAddress.get();
    auto sock_addr_ptr = reinterpret_cast<const ::sockaddr *>(&sock_addr);
    int result = ::bind(socket_.get(), sock_addr_ptr, sizeof(sock_addr));
    if (result < 0) {
        using namespace std::string_literals;
        throw std::runtime_error("Cannot bind to address "s +
                                 std::strerror(errno));
    }

    result = ::listen(socket_.get(), 10);
    if (result < 0) {
        using namespace std::string_literals;
        throw std::runtime_error("Cannot mark socket as listen "s +
                                 std::strerror(errno));
    }
}

void Server::Run() {
    while (true) {
        struct sockaddr clientaddress;
        socklen_t address_len = sizeof(clientaddress);

        int fd = ::accept(socket_.get(), &clientaddress, &address_len);
        if (fd < 0) {
            using namespace std::string_literals;
            throw std::runtime_error("Cannot accept new connection "s +
                                     std::strerror(errno));
        }

        std::cout << "Client get connect by socketaddr: ";
        for (auto i : clientaddress.sa_data)
            std::cout << i;
        Protocol p;
        p.ReciveFile(fd, f_s_);
        // connection.write_str("Hello world!");
    }
}