#include "protocol.hpp"
#include "connection.hpp"
#include "file_system.hpp"

#include <iostream>

void Protocol::SendFile(std::string_view path, std::string_view ip) {
    auto file_base = std::fstream();
    auto f_s = fs::FileSystem(file_base);
    auto fid = fs::F::FID{"aadsfa"};
    f_s.selectNewReadFile(fid);
    size_t size = f_s.getSizeFileRead();
    Connection con{SocketAddress{std::string{ip}, 8080}};
    // std::cout << "start work";
    for (size_t cur_size = 0; cur_size < size;) {
        fs::Buffer buf = f_s.getBuf();
        // std::cout << "work" << buf.size_ << std::endl;
        con.write(buf.buf_, buf.size_);
        cur_size += buf.size_;
    }
}

void Protocol::ReciveFile(int fd) {
    Socket s{fd};
    Connection connection{std::move(s)};

    char *const data = connection.read(fs::STANDARD_BUFFER_SIZE);
    for (size_t i = 0; i < fs::STANDARD_BUFFER_SIZE; ++i) {
        std::cout << data[i];
    }
    std::cout << "\n";
}