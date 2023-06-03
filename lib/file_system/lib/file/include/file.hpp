#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "../../iserializable/include/iserializable.hpp"

namespace file_fs {
using Path = std::filesystem::path;

struct FID : public iserial::ISerializable {
    FID();
    FID(std::string hash);

    bool operator<(const FID &other) const;
    bool operator==(const FID &other) const;

    buf::Buffer serialize() const override;
    size_t deserialize(const char *buf) override;

    std::string hash_;
};

struct FileInfo : public iserial::ISerializable {
    FileInfo();
    FileInfo(std::string description, size_t size);

    buf::Buffer serialize() const override;
    size_t deserialize(const char *buf) override;

    std::string description_;
    size_t size_;
};

class File : public iserial::ISerializable {
  public:
    File();
    File(Path path, FileInfo info);

    buf::Buffer serialize() const override;
    size_t deserialize(const char *buf) override;

    Path path_;
    FileInfo info_;
};

}   // namespace file_fs