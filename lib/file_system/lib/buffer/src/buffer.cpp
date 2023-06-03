#include "buffer.hpp"

#include <memory>
#include <stdexcept>

namespace buf {
// Buffer(size_t size) can throw std::runtime_error if size==0
Buffer::Buffer(size_t size) : size_(size) {
    if (size == 0) {
        throw std::runtime_error("can`t create buffer with size=0");
    } else {
        buf_ = new char[size];
    }
};

// Buffer(char *buf, size_t size) can throw std::runtime_error if size==0 or
// buf==nullptr
Buffer::Buffer(const char *buf, size_t size) {
    if (buf != nullptr && size != 0) {
        copyBuf(buf, buf_, size);
    }
    throw std::runtime_error("can`t create buffer with size=" +
                             std::to_string(size) + " and buf=" + buf);
}

Buffer::Buffer(Buffer &&other) {
    size_ = other.size_;
    buf_ = std::exchange(other.buf_, nullptr);
};

Buffer &Buffer::operator=(Buffer &&other) {
    if (this != &other) {
        this->~Buffer();
        size_ = other.size_;
        buf_ = std::exchange(other.buf_, nullptr);
    }

    return *this;
};

Buffer &Buffer::operator=(const Buffer &other) {
    if (this != &other) {
        char *tmp = new char[other.size_];
        copyBuf(other.buf_, tmp, other.size_);
        delete[] buf_;

        buf_ = tmp;
        size_ = other.size_;
    }
    return *this;
};

Buffer::~Buffer() { delete[] buf_; }
}   // namespace buf