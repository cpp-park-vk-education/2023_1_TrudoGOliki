#pragma once

#include "../../buffer/include/buffer.hpp"

namespace iserial {
class ISerializable {
  public:
    virtual buf::Buffer serialize() const = 0;

    // deserialize() return count bytes used in deserialize
    virtual size_t deserialize(const char *buf) = 0;

    virtual ~ISerializable() = default;
};
}   // namespace iserial
