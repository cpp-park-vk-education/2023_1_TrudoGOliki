#include "buffer.hpp"

#include <memory>

namespace buf {
// Buffer(size_t size) can throw BufError if size==0
Buffer::Buffer(size_t size) : size_(size) {
    if (size == 0) {
        throw BufError("can`t create buffer with size=0");
    } else {
        buf_ = std::make_unique<char[]>(size);
    }
};

// Buffer(char *buf, size_t size) can throw BufError if size==0 or
// buf==nullptr
Buffer::Buffer(const char *buf, size_t size) : Buffer(size) {
    if (buf == nullptr){
        throw BufError("can`t create buffer==nullptr");
    }
    if (buf != nullptr) {
        copyBuf(buf, buf_.get(), size);
        return;
    }
}

// Buffer(const &) can throw BufError
Buffer::Buffer(const Buffer &other)
    : size_(other.size_) {
    if (other.buf_ == nullptr || other.size_ == 0) {
        throw BufError("Buffer(const &) can`t work with unccorect bufers");
    }
    buf_ = std::make_unique<char[]>(other.size_);
    copyBuf(other.buf_.get(), buf_.get(), other.size_);
};

// Buffer(&&) can throw BufError
Buffer::Buffer(Buffer &&other) {
    if (other.buf_ == nullptr || other.size_ == 0) {
        throw BufError("Buffer(&&) can`t work with unccorect bufers");
    }
    size_ = other.size_;
    other.size_ = 0;
    buf_ = std::move(other.buf_);
};

// operator=(const &) can throw BufError
Buffer &Buffer::operator=(const Buffer &other) {
    if (buf_ == nullptr || other.buf_ == nullptr || size_ == 0 ||
        other.size_ == 0) {
        throw BufError("operator=(const &) can`t work with unccorect bufers");
    }
    if (this != &other) {
        auto tmp = std::make_unique<char[]>(other.size_);
        copyBuf(other.buf_.get(), tmp.get(), other.size_);

        buf_ = std::move(tmp);
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
        other.size_ = 0;
        buf_ = std::move(other.buf_);
    }

    return *this;
};

bool Buffer::operator==(const Buffer& other) const {
    if (size_ != other.size_){
        return false;
    }

    for (size_t i = 0; i < size_; i++) {
        if (buf_[i] != other.buf_[i]) {
            return false;
        }
    };

    return true;
}

// Buffer::operator+ throw BufError
Buffer &Buffer::operator+(Buffer &other) {
    if (buf_ == nullptr || other.buf_ == nullptr || size_ == 0 ||
        other.size_ == 0) {
        throw BufError("can`t summ unccorect bufers");
    }
    auto tmp = std::make_unique<char[]>(size_ + other.size_);
    copyBuf(buf_.get(), tmp.get(), size_);
    copyBuf(other.buf_.get(), tmp.get() + size_, other.size_);

    buf_ = std::move(tmp);
    size_ += other.size_;

    return *this;
};

char *Buffer::get() const noexcept { return buf_.get(); };
}   // namespace buf