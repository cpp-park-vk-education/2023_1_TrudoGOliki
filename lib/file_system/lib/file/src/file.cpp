#include "file.hpp"

namespace file_fs {
bool FID::operator<(const FID &other) const {
    if (hash_ < other.hash_) {
        return true;
    }

    return false;
};

bool FID::operator==(const FID &other) const { return hash_ == other.hash_; };

std::string FID::string() const { return hash_; };

}   // namespace file_fs