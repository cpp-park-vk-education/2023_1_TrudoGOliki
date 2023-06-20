#pragma once

#include <memory>
#include <stddef.h>
#include <stdexcept>

namespace buf {
class BufError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

inline void copyBuf(const char *from_buf, char *to_buf, size_t count) {
    for (size_t i = 0; i < count; i++) {
        to_buf[i] = from_buf[i];
    }
};

class Buffer {
  public:
    explicit Buffer(size_t size);
    explicit Buffer(const char *buf, size_t size);
    Buffer(const Buffer &other);
    Buffer(Buffer &&other);
    Buffer &operator=(const Buffer &other);
    Buffer &operator=(Buffer &&other);
    Buffer &operator+(Buffer &other);

    bool operator==(const Buffer& other) const;

    char *get() const noexcept;

    std::unique_ptr<char[]> buf_;
    size_t size_;
};
}   // namespace buf
