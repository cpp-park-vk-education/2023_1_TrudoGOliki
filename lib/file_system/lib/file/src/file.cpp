#include <cstring>

#include "file.hpp"

namespace file_fs {
FID::FID() noexcept {};

FID::FID(std::string hash) noexcept : hash_(std::move(hash)){};

bool FID::operator<(const FID &other) const noexcept {
    if (hash_ < other.hash_) {
        return true;
    }

    return false;
};

bool FID::operator==(const FID &other) const noexcept {
    return hash_ == other.hash_;
};

buf::Buffer FID::serialize() const {
    size_t size_hash = hash_.size();
    auto buf = buf::Buffer(size_hash + sizeof(size_t));
    char *cur_position = buf.get();

    std::memcpy(cur_position, &size_hash, sizeof(size_t));
    cur_position += sizeof(size_t);
    buf::copyBuf(hash_.c_str(), cur_position, size_hash);
    cur_position += size_hash;

    return buf;
};

size_t FID::deserialize(const char *buf) {
    size_t size_hash;
    const char *cur_position = buf;
    std::memcpy(&size_hash, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    hash_ = std::string(cur_position, size_hash);
    cur_position += size_hash;

    return cur_position - buf;
};

FileInfo::FileInfo() noexcept {};

FileInfo::FileInfo(const std::string &description, size_t size) noexcept
    : description_(description), size_(size){};

FileInfo::FileInfo(std::string &&description, size_t size) noexcept
    : description_(std::move(description)), size_(size){};

buf::Buffer FileInfo::serialize() const {
    size_t desc_size = description_.size();
    auto buf = buf::Buffer(desc_size + 2 * sizeof(size_t));
    char *cur_position = buf.get();
    std::memcpy(cur_position, &desc_size, sizeof(size_t));
    cur_position += sizeof(size_t);
    buf::copyBuf(description_.c_str(), cur_position, desc_size);
    cur_position += desc_size;

    std::memcpy(cur_position, &size_, sizeof(size_t));

    return buf;
};

size_t FileInfo::deserialize(const char *buf) {
    size_t desc_size;
    const char *cur_position = buf;
    std::memcpy(&desc_size, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    description_ = std::string(cur_position, desc_size);
    cur_position += desc_size;

    std::memcpy(&size_, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);

    return cur_position - buf;
};

File::File() noexcept {};

File::File(const Path &path, const FileInfo &info) noexcept
    : path_(path), info_(info){};

File::File(Path &&path, FileInfo &&info) noexcept
    : path_(std::move(path)), info_(std::move(info)){};

buf::Buffer File::serialize() const {
    std::string path_str = path_.string();
    size_t path_size = path_str.size();

    auto buf = buf::Buffer(path_size + sizeof(size_t));
    char *cur_position = buf.get();

    std::memcpy(cur_position, &path_size, sizeof(size_t));
    cur_position += sizeof(size_t);
    buf::copyBuf(path_str.c_str(), cur_position, path_size);
    cur_position += path_size;

    auto buf_info = info_.serialize();
    buf = buf + buf_info;

    return buf;
};

size_t File::deserialize(const char *buf) {
    size_t path_size;
    const char *cur_position = buf;
    std::memcpy(&path_size, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    std::string path_str(cur_position, path_size);
    cur_position += path_size;
    path_ = Path(path_str);

    cur_position += info_.deserialize(cur_position);

    return cur_position - buf;
};

}   // namespace file_fs