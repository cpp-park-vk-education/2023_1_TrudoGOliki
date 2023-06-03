#include <cstring>

#include "file.hpp"

namespace file_fs {
FID::FID(){};

FID::FID(std::string hash) : hash_(std::move(hash)){};

bool FID::operator<(const FID &other) const {
    if (hash_ < other.hash_) {
        return true;
    }

    return false;
};

bool FID::operator==(const FID &other) const { return hash_ == other.hash_; };

buf::Buffer FID::serialize() const {
    size_t size_hash = hash_.size();
    auto buf = buf::Buffer(size_hash + sizeof(size_t));
    char *cur_positon = buf.buf_;

    std::memcpy(cur_positon, &size_hash, sizeof(size_t));
    cur_positon += sizeof(size_t);
    buf::copyBuf(hash_.c_str(), cur_positon, size_hash);
    cur_positon += size_hash;

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

FileInfo::FileInfo() : size_(0){};

FileInfo::FileInfo(std::string description, size_t size)
    : description_(std::move(description)), size_(size){};

File::File(){};

File::File(Path path, FileInfo info)
    : path_(std::move(path)), info_(std::move(info)){};

buf::Buffer File::serialize() const {
    std::string path_str = path_.string();
    size_t path_size = path_str.size();

    std::string description = info_.description_;
    size_t desc_size = description.size();

    auto buf = buf::Buffer(desc_size + path_size + 3 * sizeof(size_t));
    char *cur_positon = buf.buf_;

    std::memcpy(cur_positon, &path_size, sizeof(size_t));
    cur_positon += sizeof(size_t);
    buf::copyBuf(path_str.c_str(), cur_positon, path_size);
    cur_positon += path_size;

    std::memcpy(cur_positon, &desc_size, sizeof(size_t));
    cur_positon += sizeof(size_t);
    buf::copyBuf(description.c_str(), cur_positon, desc_size);
    cur_positon += desc_size;

    std::memcpy(cur_positon, &info_.size_, sizeof(size_t));

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

    size_t desc_size;
    std::memcpy(&desc_size, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    info_.description_ = std::string(cur_position, desc_size);
    cur_position += desc_size;

    std::memcpy(&info_.size_, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);

    return cur_position - buf;
};

}   // namespace file_fs