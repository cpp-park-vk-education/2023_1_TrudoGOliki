#include <iostream>

#include "file.hpp"
#include "file_system.hpp"
#include "main_dir.hpp"

int main() {
    // Здесь я просто быстро проверяю, работают ли мои функции
    auto f_s = fs::FileSystem(
        fs::NAME_MAIN_DIR, std::make_unique<fs::AVLTreeSearch>(),
        std::make_unique<fs::ReaderNet>(), std::make_unique<fs::WriterNet>(),
        std::make_unique<fs::ManagerFilesCLI>());
    auto fid = f_s.addFile(
        file_fs::Path("/home/vilin/techno_park/2023_1_TrudoGOliki/lib/"
                      "file_system/lib/avl_tree/include/avl_tree.hpp"),
        "its my file");
    f_s.selectNewReadFile(fid);
    auto size = f_s.getSizeFileRead();
    f_s.createNewFileWrite(file_fs::FID{"a"},
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

    auto res = f_s.getAllFids();
    for (int i = 0; i < res.size(); i++) {
        std::cout << res[i].hash_ << std::endl;
    }
    std::cout << std::endl << "_________" << std::endl;

    auto full_res = f_s.getAll();
    for (int i = 0; i < full_res.size(); i++) {
        std::cout << full_res[i].first.hash_ << " "
                  << full_res[i].second.info_.description_ << " "
                  << full_res[i].second.info_.size_ << std::endl;
    }

    return 0;
}
