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
    std::string_view hash_type_;
    std::string_view content_type_;
    std::string_view base_type_;
};

struct FID {
    FIDInfo info_;
    std::string_view hash_;
};

struct FileInfo {
    std::vector<BlockDataHash> blocks_info_;
    size_t max_block_size_;
};

struct BlockData {
    Path path_;
};

class File {
  public:
    std::vector<BlockData> blocks_;
    std::unordered_map<BlockDataHash, IdxBlock> map_block_info;
    FileInfo info_;

    Path &getPath(BlockDataHash hash);

  private:
    IdxBlock checkHash(BlockDataHash hash);
};

}   // namespace file_for_fs