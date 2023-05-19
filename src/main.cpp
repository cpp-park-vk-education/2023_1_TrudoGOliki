#include <iostream>

#include "file_system.hpp"

int main(int argc, char *argv[]) {
    auto manager_net = fs::ManagerFilesNet();
    // manager_net.selectNewFile(fs::F::Path(".gitignore"));
    fs::Buffer buf = manager_net.getBuf();
    for (int i = 0; i < fs::STANDARD_BUFFER_SIZE; i++) {
        std::cout << buf.buf_[i];
    }
}
