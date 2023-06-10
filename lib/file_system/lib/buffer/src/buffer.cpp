#include "buffer.hpp"

#include <memory>
#include <stdexcept>

namespace buf {
class BufError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

// Buffer(size_t size) can throw BufError if size==0
Buffer::Buffer(size_t size) : size_(size) {
    if (size == 0) {
        throw BufError("can`t create buffer with size=0");
    } else {
        buf_ = std::unique_ptr<char[]>(new char[size]);
    }
};

// Buffer(char *buf, size_t size) can throw BufError if size==0 or
// buf==nullptr
Buffer::Buffer(const char *buf, size_t size) : Buffer(size) {
    if (buf != nullptr) {
        copyBuf(buf, buf_.get(), size);
        return;
    }
    throw BufError("can`t create buffer with size=" + std::to_string(size) +
                   " and buf=" + buf);
}

// Buffer(const &) can throw BufError
Buffer::Buffer(const Buffer &other)
    : buf_(new char[other.size_]), size_(other.size_) {
    if (other.buf_ == nullptr || other.size_ == 0) {
        throw BufError("Buffer(const &) can`t work with unccorect bufers");
    }
    copyBuf(other.buf_.get(), buf_.get(), other.size_);
};

// Buffer(&&) can throw BufError
Buffer::Buffer(Buffer &&other) {
    if (other.buf_ == nullptr || other.size_ == 0) {
        throw BufError("Buffer(&&) can`t work with unccorect bufers");
    }
    size_ = other.size_;
    buf_ = std::exchange(other.buf_, nullptr);
};

// operator=(const &) can throw BufError
Buffer &Buffer::operator=(const Buffer &other) {
    if (buf_ == nullptr || other.buf_ == nullptr || size_ == 0 ||
        other.size_ == 0) {
        throw BufError("operator=(const &) can`t work with unccorect bufers");
    }
    if (this != &other) {
        auto tmp = std::unique_ptr<char[]>(new char[other.size_]);
        copyBuf(other.buf_.get(), tmp.get(), other.size_);
        buf_ = std::move(tmp);

        buf_ = std::exchange(tmp, nullptr);
        size_ = other.size_;
    }
    return *this;
};

// operator=(&&) can throw BufError
Buffer &Buffer::operator=(Buffer &&other) {
    if (buf_ == nullptr || other.buf_ == nullptr || size_ == 0 ||
        other.size_ == 0) {
        throw BufError("operator=(&&) can`t work with unccorect bufers");
    }
    if (this != &other) {
        this->~Buffer();
        size_ = other.size_;
        buf_ = std::exchange(other.buf_, nullptr);
    }

    return *this;
};

// Buffer::operator+ throw BufError
Buffer &Buffer::operator+(Buffer &other) {
    if (buf_ == nullptr || other.buf_ == nullptr || size_ == 0 ||
        other.size_ == 0) {
        throw BufError("can`t summ unccorect bufers");
    }
    auto tmp = std::unique_ptr<char[]>(new char[size_ + other.size_]);
    copyBuf(buf_.get(), tmp.get(), size_);
    copyBuf(other.buf_.get(), tmp.get() + size_, other.size_);

    buf_ = std::move(tmp);
    size_ += other.size_;

    return *this;
};

char *Buffer::get() const noexcept { return buf_.get(); };
}   // namespace buf