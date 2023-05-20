#include <iostream>

#include "file_system.hpp"

int main(int argc, char *argv[]) {
    auto file_base = std::fstream();
    auto f_s = fs::FileSystem(file_base);
    auto fid = fs::F::FID{"a"};
    f_s.selectNewReadFile(fid);
    std::cout << f_s.getSizeFileRead() << std::endl;
    std::cout << f_s.getBuf().buf_;
}
