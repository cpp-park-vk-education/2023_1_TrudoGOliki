#pragma once

#include <filesystem>
#include <string>

#include "../../iserializable/include/iserializable.hpp"

namespace file_fs {
using Path = std::filesystem::path;

struct FID : public iserial::ISerializable {
    explicit FID() noexcept;
    explicit FID(std::string hash) noexcept;

    bool operator<(const FID &other) const noexcept;
    bool operator==(const FID &other) const noexcept;

    buf::Buffer serialize() const override;
    size_t deserialize(const char *buf) override;

    std::string hash_;
};

struct FileInfo : public iserial::ISerializable {
    explicit FileInfo() noexcept;
    explicit FileInfo(const std::string &description, size_t size) noexcept;
    explicit FileInfo(std::string &&description, size_t size) noexcept;

    buf::Buffer serialize() const override;
    size_t deserialize(const char *buf) override;

    std::string description_;
    size_t size_;
};

class File : public iserial::ISerializable {
  public:
    explicit File() noexcept;
    explicit File(const Path &path, const FileInfo &info) noexcept;
    explicit File(Path &&path, FileInfo &&info) noexcept;

    buf::Buffer serialize() const override;
    size_t deserialize(const char *buf) override;

    Path path_;
    FileInfo info_;
};

}   // namespace file_fs