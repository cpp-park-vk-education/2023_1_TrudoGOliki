#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace file_for_fs {
using BlockDataHash = std::string_view;
using IdxBlock = size_t;
using Path = std::string;

struct FIDInfo {
    const std::string hash_type_;
    const std::string content_type_;
    const std::string base_type_;

    bool operator<(const FIDInfo &other) const;
    bool operator==(const FIDInfo &other) const;
};

struct FID {
    const FIDInfo info_;
    const std::string hash_;
    bool operator<(const FID &other) const;
    bool operator==(const FID &other) const;
};

struct BlockData {
    BlockData(const Path &path) : path_(path){};
    Path path_;
};

struct FileInfo {
    std::vector<BlockDataHash> blocks_info_;
    size_t max_block_size_;
};

class File {
  public:
    std::vector<BlockData> blocks_;
    std::unordered_map<BlockDataHash, IdxBlock> map_block_info;
    FileInfo info_;

    Path &getPath(const BlockDataHash &hash);

  private:
};

}   // namespace file_for_fs