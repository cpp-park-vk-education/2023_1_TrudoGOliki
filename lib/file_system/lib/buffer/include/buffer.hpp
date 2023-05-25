#pragma once

#include <stddef.h>

namespace buf {
class Buffer {
  public:
    explicit Buffer(size_t size);
    Buffer(const Buffer &other)
        : buf_(new char[other.size_]), size_(other.size_) {
        copyBuf(other.buf_, this->buf_, other.size_);
    };
    Buffer(Buffer &&other);
    Buffer &operator=(const Buffer &other);
    Buffer &operator=(Buffer &&other);

    ~Buffer();

    char *buf_;
    size_t size_;

  private:
    void copyBuf(char *buf1, char *buf2, size_t count);
};
}   // namespace buf
