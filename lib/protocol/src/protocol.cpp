#include "protocol.hpp"
#include "connection.hpp"

#include <iostream>

void Protocol::SendFile(std::string_view path, std::string_view ip) {
    auto file_base = std::fstream();

    auto f_s = fs::FileSystem(file_base, std::string{fs::NAME_MAIN_DIR});
    auto fid = fs::F::FID{std::string{path}};

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
    auto file_info = fs::F::FileInfo{"asdfasdf", 1439};
    f_s.createNewFileWrite(fid, file_info);

    for (size_t cur_size = 0; cur_size < file_info.size_;) {
        size_t cur_read_count = file_info.size_ < fs::STANDARD_BUFFER_SIZE
                                    ? file_info.size_
                                    : fs::STANDARD_BUFFER_SIZE;
        fs::Buffer data = connection.read(cur_read_count);
        fs::Buffer buf = {data.buf_, cur_read_count};
        if (data.buf_) {
            f_s.writeBuf(buf);
            for (size_t j = 0; j < cur_read_count; ++j) {
                std::cout << data.buf_[j];
            }
            cur_size += cur_read_count;
        }
    }

    std::cout << "\n";
}