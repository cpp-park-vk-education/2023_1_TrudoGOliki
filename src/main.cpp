#include <iostream>

#include "file_system.hpp"

int main(int argc, char *argv[]) {
    auto file_base = std::fstream();
    auto f_s = fs::FileSystem(file_base, fs::NAME_MAIN_DIR);
    auto fid = f_s.addFile(
        fs::F::Path("/home/vilin/Pictures/"
                    "imgonline-com-ua-GifAnimationTwoPic-u1BuW6Xz9gFp.gif"),
        "its my file");
    // f_s.eraseFile(fid);
    f_s.selectNewReadFile(fid);
    auto size = f_s.getSizeFileRead();

    buf::Buffer buf = f_s.getBuf();
    size_t cur_size = 0;
    while (buf.buf_) {
        try {
            cur_size += buf.size_;
            std::cout << "in while" << buf.size_ << std::endl;
            std::cout << "text: " << std::endl;
            for (int i = 0; i < buf.size_; i++) {
                std::cout << buf.buf_[i];
            }
            buf = f_s.getBuf();
        } catch (std::exception &e) {
            std::cerr << "Exception: " << e.what();
            break;
        }
    }
    if (cur_size != size) {
        throw "cur_size != size";
    }
    return 0;
}
