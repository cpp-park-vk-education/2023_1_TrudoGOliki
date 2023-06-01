#include <iostream>

#include "constants.hpp"
#include "errors.hpp"
#include "manager_files_net.hpp"

namespace fs {
// WriterNet::createNewFileWrite() can throw FSError
void WriterNet::createNewFileWrite(const file_fs::FID &fid,
                                   const file_fs::File &file) {
    if (of_stream_.is_open()) {
        throw FSError("in createNewFileWrite: another file already open");
    }   // namespace fs

    size_file_ = file.info_.size_;
    std::string path_str = file.path_.string();
    try {
        of_stream_.open(path_str, std::ios::binary | std::ios::out);
        if (!of_stream_.is_open()) {
            throw FSError("file not opened. Path=" + path_str +
                          " Fid=" + fid.string());
        }
    } catch (std::ios::failure &e) {
        throw FSError("in selectNewFileRead exception: " +
                      static_cast<std::string>(e.what()));
    }
}

void WriterNet::writeBuf(const buf::Buffer &buf) {
    if (of_stream_.tellp() < size_file_) {
        of_stream_.write(buf.buf_, buf.size_);
        if (of_stream_.tellp() == size_file_) {
            of_stream_.close();
        }
    } else {
        throw FSError("attempt write more than size of file ");
    }
};

// ReaderNet::selectNewFileRead() can throw FSError
void ReaderNet::selectNewFileRead(const file_fs::Path &path) {
    if (f_stream_.is_open()) {
        throw FSError("in selectNewFileRead: another file already open");
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

// ReaderNet::getBuf() can throw FSError and return buf::Buffer{buf_ ==
// nullptr and size_ = 0, when read of file done
buf::Buffer ReaderNet::getBuf() {
    try {
        if (f_stream_.tellg() < size_file_) {
            buf::Buffer buffer = buf::Buffer(STANDARD_BUFFER_SIZE);
            f_stream_.read(buffer.buf_, STANDARD_BUFFER_SIZE);
            auto remainder = f_stream_.gcount();
            if (remainder == 0) {
                return buf::Buffer(0);
            }
            if (remainder < 0) {
                throw FSError("ramainder < 0. Why?");
            }
            std::cout << "in read buf" << remainder << std::endl;
            buffer.size_ = static_cast<size_t>(remainder);

            return buffer;
        }
        if (f_stream_.eof()) {
            f_stream_.close();
        }
        return buf::Buffer(0);
    } catch (std::ios::failure &e) {
        throw FSError("in getBuf exception: " +
                      static_cast<std::string>(e.what()));
    }
}

void ReaderNet::setSizeRead(size_t size) { size_file_ = size; };

size_t ReaderNet::getSizeFileRead() const { return size_file_; }

}   // namespace fs
