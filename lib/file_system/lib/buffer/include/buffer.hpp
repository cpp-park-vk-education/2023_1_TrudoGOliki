#pragma once

#include <memory>
#include <stddef.h>

namespace buf {
inline void copyBuf(const char *from_buf, char *to_buf, size_t count) {
    for (size_t i = 0; i < count; i++) {
        to_buf[i] = from_buf[i];
    }
};

class Buffer {
  public:
    explicit Buffer(size_t size);
    Buffer(const char *buf, size_t size);
    Buffer(const Buffer &other);
    Buffer(Buffer &&other);
    Buffer &operator=(const Buffer &other);
    Buffer &operator=(Buffer &&other);
    Buffer &operator+(Buffer &other);

    char *get() const noexcept;

    std::unique_ptr<char[]> buf_;
    size_t size_;
};
}   // namespace buf
