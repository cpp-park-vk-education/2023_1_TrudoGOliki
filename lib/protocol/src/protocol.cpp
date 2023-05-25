#include "protocol.hpp"
#include "connection.hpp"

#include <iostream>

// SendFile can throw fs::FSError
void Protocol::SendFile(std::string_view path, std::string_view ip) {
    auto file_base = std::fstream();

    auto f_s = fs::FileSystem(file_base, std::string{fs::NAME_MAIN_DIR});
    auto fid = fs::F::FID{std::string{path}};

    f_s.selectNewReadFile(fid);
    size_t size = f_s.getSizeFileRead();
    std::cout << "SIZE in send" << size;
    Connection con{SocketAddress{std::string{ip}, 8080, INADDR_LOOPBACK}};
    std::cout << "start work" << std::endl;
    size_t cur_size = 0;
    buf::Buffer buf = f_s.getBuf();
    while (buf.buf_) {
        con.write(buf.buf_, buf.size_);
        cur_size += buf.size_;
        std::cout << "work" << buf.size_ << std::endl;
        buf = f_s.getBuf();
    }
    if (cur_size != size) {
        throw std::runtime_error("in SendFile: read not full file " +
                                 std::to_string(cur_size) +
                                 " != " + std::to_string(size));
    }

    std::cout << "end work";
};

void Protocol::ReciveFile(int fd, fs::FileSystem &f_s) {
    Socket s{fd};
    Connection connection{std::move(s)};

    auto fid = fs::F::FID{"aaaa"};
    auto file_info = fs::F::FileInfo{"asdfasdf", 7356};
    std::cout << "SIZE in Recive before" << file_info.size_;
    f_s.createNewFileWrite(fid, file_info);
    size_t cur_size = 0;
    for (; cur_size < file_info.size_;) {
        size_t cur_read_count =
            (file_info.size_ - cur_size) < fs::STANDARD_BUFFER_SIZE
                ? file_info.size_ - cur_size
                : fs::STANDARD_BUFFER_SIZE;
        buf::Buffer data = connection.read(cur_read_count);
        if (data.buf_) {
            f_s.writeBuf(data);
            std::cout << "Write " << data.size_;
            cur_size += cur_read_count;
        }
    }
    std::cout << "SIZE CUR in Recive " << cur_size;
    if (cur_size != file_info.size_) {
        throw std::runtime_error("in ReciveFile: cur_size != expexted size " +
                                 std::to_string(cur_size) +
                                 " != " + std::to_string(file_info.size_));
    }

    std::cout << "end recieve\n";
}