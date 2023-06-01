#include "file.hpp"

namespace file_fs {
FileInfo::FileInfo() : size_(0){};
FileInfo::FileInfo(std::string description, size_t size)
    : description_(std::move(description)), size_(size){};

bool FID::operator<(const FID &other) const {
    if (hash_ < other.hash_) {
        return true;
    }

    return false;
};

bool FID::operator==(const FID &other) const { return hash_ == other.hash_; };

std::string FID::string() const { return hash_; };

}   // namespace file_fs