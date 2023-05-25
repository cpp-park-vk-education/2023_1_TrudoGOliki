#include <iostream>

#include "file_system.hpp"

int main(int argc, char *argv[]) {
    auto file_base_1 = std::fstream();
    auto f_s_1 =
        fs::FileSystem(file_base_1, "techno_park/2023_1_TrudoGOliki/build");
    // f_s_1.manager_cli_.copyFile(
    //     fs::F::FID(),
    //     "/home/vilin/techno_park/2023_1_TrudoGOliki/build/yazyk_go.pdf",
    //     "/home/vilin/techno_park/2023_1_TrudoGOliki/build/test.pdf");
    f_s_1.selectNewReadFile(fs::F::FID());
    auto size = f_s_1.getSizeFileRead();
    auto file_base = std::fstream();

    auto f_s =
        fs::FileSystem(file_base, "techno_park/2023_1_TrudoGOliki/build");
    auto fid = fs::F::FID{"test"};
    auto file_info = fs::F::FileInfo{"asdfasdf", size};
    f_s.createNewFileWrite(fid, file_info);
    fs::Buffer buf = f_s_1.getBuf();
    while (buf.buf_) {
        try {
            f_s.writeBuf(buf);
            std::cout << "in while" << buf.size_ << std::endl;
            buf = f_s_1.getBuf();
        } catch (std::exception &e) {
            std::cerr << "Exception: " << e.what();
            break;
        }
    }

    // char buffer[100];
    // for (int i = 0; i < 100; i++) {
    //     buffer[i] = 'e';
    // }
    // auto buf = fs::Buffer{buffer, 100};
    // f_s.writeBuf(buf);
    // f_s.writeBuf(buf);
    // f_s.writeBuf(buf);

    // size_t size = f_s.getSizeFileRead();
    // auto out = std::ofstream("../build/output.pdf", std::ios::binary);
    // for (size_t cur_size = 0; cur_size < size;) {
    //     fs::Buffer buf = f_s.getBuf();
    //     out.write(buf.buf_, buf.size_);
    //     cur_size += buf.size_;
    // }
}
