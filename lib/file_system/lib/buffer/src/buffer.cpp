#include "buffer.hpp"

#include <memory>
#include <stdexcept>

#include "../../errors/include/errors.hpp"

namespace buf {
// Buffer(size_t size) can throw fs::BufError if size==0
Buffer::Buffer(size_t size) : size_(size) {
    if (size == 0) {
        throw fs::BufError("can`t create buffer with size=0");
    } else {
        buf_ = new char[size];
    }
};

// Buffer(char *buf, size_t size) can throw fs::BufError if size==0 or
// buf==nullptr
Buffer::Buffer(const char *buf, size_t size) : Buffer(size) {
    if (buf != nullptr) {
        copyBuf(buf, buf_, size);
        return;
    }
    throw fs::BufError("can`t create buffer with size=" + std::to_string(size) +
                       " and buf=" + buf);
}

// Buffer(const &) can throw fs::BufError
Buffer::Buffer(const Buffer &other)
    : buf_(new char[other.size_]), size_(other.size_) {
    if (other.buf_ == nullptr || other.size_ == 0) {
        throw fs::BufError("Buffer(const &) can`t work with unccorect bufers");
    }
    copyBuf(other.buf_, this->buf_, other.size_);
};

// Buffer(&&) can throw fs::BufError
Buffer::Buffer(Buffer &&other) {
    if (other.buf_ == nullptr || other.size_ == 0) {
        throw fs::BufError("Buffer(&&) can`t work with unccorect bufers");
    }
    size_ = other.size_;
    buf_ = std::exchange(other.buf_, nullptr);
};

// operator=(const &) can throw fs::BufError
Buffer &Buffer::operator=(const Buffer &other) {
    if (buf_ == nullptr || other.buf_ == nullptr || size_ == 0 ||
        other.size_ == 0) {
        throw fs::BufError(
            "operator=(const &) can`t work with unccorect bufers");
    }
    if (this != &other) {
        char *tmp = new char[other.size_];
        copyBuf(other.buf_, tmp, other.size_);
        delete[] buf_;

        buf_ = std::exchange(tmp, nullptr);
        size_ = other.size_;
    }
    return *this;
};

// operator=(&&) can throw fs::BufError
Buffer &Buffer::operator=(Buffer &&other) {
    if (buf_ == nullptr || other.buf_ == nullptr || size_ == 0 ||
        other.size_ == 0) {
        throw fs::BufError("operator=(&&) can`t work with unccorect bufers");
    }
    if (this != &other) {
        this->~Buffer();
        size_ = other.size_;
        buf_ = std::exchange(other.buf_, nullptr);
    }

    return *this;
};

// Buffer::operator+ throw fs::BufError
Buffer &Buffer::operator+(Buffer &other) {
    if (buf_ == nullptr || other.buf_ == nullptr || size_ == 0 ||
        other.size_ == 0) {
        throw fs::BufError("can`t summ unccorect bufers");
    }
    Buffer tmp(size_ + other.size_);
    copyBuf(buf_, tmp.buf_, size_);
    copyBuf(other.buf_, tmp.buf_ + size_, other.size_);

    delete[] buf_;
    buf_ = std::exchange(tmp.buf_, nullptr);
    size_ = tmp.size_;

    return *this;
};

Buffer::~Buffer() { delete[] buf_; }
}   // namespace buf