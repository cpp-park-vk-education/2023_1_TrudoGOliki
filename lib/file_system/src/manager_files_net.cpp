#include <iostream>

#include "constants.hpp"
#include "errors.hpp"
#include "manager_files_net.hpp"

namespace fs {

void ManagerFilesNet::setSizeRead(size_t size) { size_file_ = size; };

// ManagerFilesNet::selectNewFileRead() can throw FSError
void ManagerFilesNet::selectNewFileRead(const file_fs::Path &path) {
    if (f_stream_.is_open()) {
        f_stream_.close();
    }

    try {
        f_stream_.open(path.string(), std::ios::binary | std::ios::in);
        if (!f_stream_.is_open()) {
            throw FSError("file not opened");
        }
    } catch (std::ios::failure &e) {
        throw FSError("in selectNewFileRead exception: " +
                      static_cast<std::string>(e.what()));
    }
}

size_t ManagerFilesNet::getSizeFileRead() const { return size_file_; }

// ManagerFilesNet::getBuf() can throw FSError and return buf::Buffer{buf_ ==
// nullptr and size_ = 0, when read of file done
buf::Buffer ManagerFilesNet::getBuf() {
    try {
        if (f_stream_.tellg() < size_file_) {
            buf::Buffer buffer = buf::Buffer(STANDARD_BUFFER_SIZE);
            f_stream_.read(buffer.buf_, STANDARD_BUFFER_SIZE);
            auto remainder = f_stream_.gcount();
            if (remainder == 0) {
                return buf::Buffer(0);
            }
            if (remainder < 0) {
                throw FSError("BLLL");
            }
            std::cout << "in read buf" << remainder << std::endl;
            buffer.size_ = static_cast<size_t>(remainder);
            return buffer;
        }
        return buf::Buffer(0);
    } catch (std::ios::failure &e) {
        throw FSError("in getBuf exception: " +
                      static_cast<std::string>(e.what()));
    }
}

// ManagerFileNet::createNewFileWrite() can throw FSError
void ManagerFilesNet::createNewFileWrite(const file_fs::FID &fid,
                                         const file_fs::File &file) {
    if (f_stream_.is_open()) {
        f_stream_.close();
    }

    size_file_ = file.info_.size_;
    std::string path_str = file.path_.string();
    try {
        f_stream_.open(path_str, std::ios::binary | std::ios::out);
        if (!f_stream_.is_open()) {
            throw FSError("file not opened");
        }
    } catch (std::ios::failure &e) {
        throw FSError("in selectNewFileRead exception: " +
                      static_cast<std::string>(e.what()));
    }
}

void ManagerFilesNet::writeBuf(const buf::Buffer &buf) {
    if (f_stream_.tellp() < size_file_) {
        f_stream_.write(buf.buf_, buf.size_);
    } else {
        throw FSError("attempt write more than size of file ");
    }
}
}   // namespace fs
