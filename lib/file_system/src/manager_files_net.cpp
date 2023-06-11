#include "manager_files_net.hpp"

#include <iostream>

#include "fs_error.hpp"
#include "standard_buffer_size.hpp"

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
                          " Fid=" + fid.hash_);
        }
    } catch (std::ios::failure &e) {
        throw FSError("in selectNewFileRead exception: " +
                      static_cast<std::string>(e.what()));
    }
}

void WriterNet::writeBuf(const buf::Buffer &buf) {
    if (of_stream_.tellp() < size_file_) {
        of_stream_.write(buf.get(), buf.size_);
        if (of_stream_.tellp() == size_file_) {
            of_stream_.close();
        }
    } else {
        throw FSError("attempt write more than size of file ");
    }
};

ReaderNet::ReaderNet() : size_file_(0){};
ReaderNet::~ReaderNet() = default;

// ReaderNet::selectNewFileRead() can throw FSError
void ReaderNet::selectNewFileRead(const file_fs::Path &path) {
    if (if_stream_.is_open()) {
        throw FSError("in selectNewFileRead: another file already open");
    }

    try {
        if_stream_.open(path.string(), std::ios::binary | std::ios::in);
        if (!if_stream_.is_open()) {
            throw FSError("file not opened");
        }
    } catch (std::ios::failure &e) {
        throw FSError("in selectNewFileRead exception: " +
                      static_cast<std::string>(e.what()));
    }
}

// ReaderNet::getBuf() can throw FSError
buf::Buffer ReaderNet::getBuf() {
    try {
        if (if_stream_.tellg() < size_file_) {
            buf::Buffer buffer = buf::Buffer(STANDARD_BUFFER_SIZE);
            if_stream_.read(buffer.get(), STANDARD_BUFFER_SIZE);
            auto remainder = if_stream_.gcount();
            if (remainder == 0) {
                throw FSError(
                    "remainder == 0. Something wrong with read from file");
            }
            if (remainder < 0) {
                throw FSError("remainder < 0. Why?");
            }
            std::cout << "in read buf" << remainder << std::endl;
            buffer.size_ = static_cast<size_t>(remainder);

            return buffer;
        }
        if (if_stream_.eof()) {
            if_stream_.close();
        }

        throw FSError("unknown error. May be you try read from closed file");
    } catch (std::ios::failure &e) {
        throw FSError("in getBuf exception: " +
                      static_cast<std::string>(e.what()));
    }
}

void ReaderNet::setSizeRead(size_t size) noexcept { size_file_ = size; };

size_t ReaderNet::getSizeFileRead() const noexcept { return size_file_; }

ManagerFilesNet::ManagerFilesNet(IReaderUP reader, IWriterUP writer)
    : reader_(std::move(reader)), writer_(std::move(writer)){};

}   // namespace fs
