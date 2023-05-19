#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace file_for_fs {
using Path = std::filesystem::path;

struct FID {
    const std::string hash_;

    bool operator<(const FID &other) const;
    bool operator==(const FID &other) const;
    std::string string() const;
};

struct FileInfo {
    std::string description_;
    size_t size_;
};

class File {
  public:
    Path path_;
    FileInfo info_;

  private:
};

}   // namespace file_for_fs