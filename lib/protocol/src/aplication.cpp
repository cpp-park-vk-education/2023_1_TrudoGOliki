#include "connection.hpp"
#include "protocol.hpp"

#include <iostream>

// SendFile can throw fs::FSError
void Protocol::SendFile(std::string_view path, std::string_view ip) {

    auto file_base = std::fstream();

    auto f_s = fs::FileSystem(file_base, std::string{fs::NAME_MAIN_DIR});
    auto fid = f_s.addFile(fs::F::Path(path), "descrip");

    f_s.selectNewReadFile(fid);
    size_t size = f_s.getSizeFileRead();
    std::cout << "SIZE in send" << size;

    Data data(size, ".fdssaf", fid.string());

    buf::Buffer buf = data.serialize();
    Connection con{SocketAddress{std::string{ip}, 8080, INADDR_LOOPBACK}};
    con.write(buf.buf_, buf.size_);

    std::cout << "start work" << std::endl;
    size_t cur_size = 0;
    buf::Buffer buf_send = f_s.getBuf();
    while (buf_send.buf_) {
        con.write(buf_send.buf_, buf_send.size_);
        cur_size += buf_send.size_;
        std::cout << "work" << buf_send.size_ << std::endl;
        buf_send = f_s.getBuf();
    }
    if (cur_size != size) {
        throw std::runtime_error("in SendFile: read not full file " +
                                 std::to_string(cur_size) +
                                 " != " + std::to_string(size));
    }
};

void Protocol::ReciveFile(int fd, fs::FileSystem &f_s) {
    Socket s{fd};
    Connection connection{std::move(s)};

    buf::Buffer buf = connection.read(fs::STANDARD_BUFFER_SIZE);
    Data data_clean(buf);

    std::cout << "data clean" << data_clean.fid << " " << data_clean.len_file
              << " " << data_clean.type_file;
    auto fid = fs::F::FID{data_clean.fid};
    auto file_info = fs::F::FileInfo{"description", data_clean.len_file};
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