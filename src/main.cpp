#include <iostream>

#include "file_system.hpp"

int main() {
    auto f_s = fs::FileSystem(fs::NAME_MAIN_DIR);
    auto fid = f_s.addFile(
        fs::F::Path("/home/vilin/techno_park/2023_1_TrudoGOliki/lib/"
                    "file_system/lib/avl_tree/include/avl_tree.hpp"),
        "its my file");
    f_s.selectNewReadFile(fid);
    auto size = f_s.getSizeFileRead();
    f_s.createNewFileWrite(file_fs::FID(),
                           file_fs::FileInfo{"description", size});

    buf::Buffer buf = f_s.getBuf();
    size_t cur_size = 0;
    while (buf.buf_) {
        try {
            cur_size += buf.size_;
            std::cout << "in while" << buf.size_ << std::endl;
            std::cout << "text: " << std::endl;
            f_s.writeBuf(buf);
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

    {
        f_s.selectNewReadFile(fid);
        auto size = f_s.getSizeFileRead();
        f_s.createNewFileWrite(file_fs::FID{"aadsf"},
                               file_fs::FileInfo{"description", size});

        buf::Buffer buf = f_s.getBuf();
        size_t cur_size = 0;
        while (buf.buf_) {
            try {
                cur_size += buf.size_;
                std::cout << "in while" << buf.size_ << std::endl;
                std::cout << "text: " << std::endl;
                f_s.writeBuf(buf);
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
    }

    return 0;
}
