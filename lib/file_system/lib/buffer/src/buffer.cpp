#include "buffer.hpp"

#include <memory>

namespace buf {
Buffer::Buffer(size_t size) : size_(size) {
    if (size == 0) {
        buf_ = nullptr;
    } else {
        buf_ = new char[size];
    }
};

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

void Buffer::copyBuf(char *buf1, char *buf2, size_t count) {
    for (int i = 0; i < count; i++) {
        buf2[i] = buf1[i];
    }
};

Buffer::~Buffer() { delete[] buf_; }
}   // namespace buf