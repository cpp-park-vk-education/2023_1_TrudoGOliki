#include "protocol.hpp"
#include "connection.hpp"

#include <iostream>

void Protocol::SendFile(std::string_view path, std::string_view ip) {
    auto file_base = std::fstream();

    auto f_s = fs::FileSystem(file_base, "park/2023_1_TrudoGOliki/build");
    auto fid = fs::F::FID{"aadsfa"};

    f_s.selectNewReadFile(fid);
    size_t size = f_s.getSizeFileRead();
    Connection con{SocketAddress{std::string{ip}, 8080, INADDR_LOOPBACK}};
    std::cout << "start work" << std::endl;
    for (size_t cur_size = 0; cur_size < size;) {
        fs::Buffer buf = f_s.getBuf();
        std::cout << "work" << buf.size_ << std::endl;
        con.write(buf.buf_, buf.size_);
        cur_size += buf.size_;
    }
    std::cout << "end work";
};

void Protocol::ReciveFile(int fd, fs::FileSystem &f_s) {
    Socket s{fd};
    Connection connection{std::move(s)};

    auto fid = fs::F::FID{"aaaa"};
    auto file_info = fs::F::FileInfo{"asdfasdf", 40960};
    f_s.createNewFileWrite(fid, file_info);

    for (int i = 0; i < 10; i++) {
        char *const data = connection.read(fs::STANDARD_BUFFER_SIZE);
        fs::Buffer buf = {data, fs::STANDARD_BUFFER_SIZE};
        f_s.writeBuf(buf);
        for (size_t j = 0; j < fs::STANDARD_BUFFER_SIZE; ++j) {
            std::cout << data[j];
        }
    }

    std::cout << "\n";
}