#include "manager_files_cli.hpp"

#include <fstream>

#include "fs_error.hpp"
#include "sha256.hpp"
#include "standard_buffer_size.hpp"

namespace fs {

file_fs::FID ManagerFilesCLI::calculFID(const file_fs::Path &path_from) {
    std::ifstream in(path_from.string());

    char buffer1[STANDARD_BUFFER_SIZE];
    char buffer2[STANDARD_BUFFER_SIZE];

    if (!in.is_open()) {
        throw FSError("in calculFID: can`t open file: " + path_from.string());
    }

    std::string prev_hash;
    while (true) {
        SHA256 sha256;
        bool flag_first_read = false;
        if (in.read(buffer1, STANDARD_BUFFER_SIZE)) {
            flag_first_read = true;
        };
        if (auto remainder = in.gcount(); remainder > 0) {
            std::string cur_hash;
            for (size_t i = 0; i < remainder; i++) {
                cur_hash += buffer1[i];
            }

            prev_hash +=
                sha256(prev_hash + sha256(cur_hash) + sha256(cur_hash));

            return file_fs::FID{prev_hash};
        }

        bool flag_second_read = false;
        if (in.read(buffer2, STANDARD_BUFFER_SIZE)) {
            flag_second_read = true;
        };
        if (auto remainder = in.gcount(); remainder > 0) {
            std::string cur_hash;
            for (size_t i = 0; i < remainder; i++) {
                cur_hash += buffer2[i];
            }

            prev_hash +=
                sha256(prev_hash + sha256(buffer1, STANDARD_BUFFER_SIZE) +
                       sha256(cur_hash));
            return file_fs::FID{prev_hash};
        }

        if (!flag_first_read && !flag_second_read) {
            return file_fs::FID{prev_hash};
        }

        prev_hash = sha256(prev_hash + sha256(buffer1, STANDARD_BUFFER_SIZE) +
                           sha256(buffer2, STANDARD_BUFFER_SIZE));
    }
};

// ManagerFilesCLI::copyFile() can throw FSError
void ManagerFilesCLI::copyFile(const file_fs::Path &path_from,
                               const file_fs::Path &path_to) {
    std::ifstream in(path_from.string(), std::ios::binary);

    std::ofstream out(path_to.string(), std::ios::binary);

    char buffer[STANDARD_BUFFER_SIZE];

    if (!in.is_open() || !out.is_open()) {
        throw FSError("in copyFile: can`t open in or out");
    }

    while (true) {
        in.read(buffer, STANDARD_BUFFER_SIZE);
        auto remainder = in.gcount();
        if (remainder == 0) {
            break;
        }
        out.write(buffer, remainder);
    }
};

// addFile() can throw FSError
void ManagerFilesCLI::addFile(const file_fs::Path &path_from,
                              const file_fs::Path &path_to) {
    try {
        copyFile(path_from, path_to);
    } catch (FSError &e) {
        throw FSError("in addFile: from " + path_from.string() + " to " +
                      path_to.string() + static_cast<std::string>(e.what()));
    }
};

// ManagerFilesCLI::eraseFile() can throw FSError
void ManagerFilesCLI::eraseFile(const file_fs::Path &path_to) {
    if (std::remove(path_to.string().c_str()) != 0) {
        throw FSError("in eraseFile: can`t remove");
    }
}
}   // namespace fs