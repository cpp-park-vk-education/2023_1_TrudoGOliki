#include "file.hpp"

namespace file_for_fs {
bool FIDInfo::operator<(const FIDInfo &other) const {
    return (hash_type_ + content_type_ + base_type_) <
           (other.hash_type_ + other.content_type_ + other.base_type_);
};

bool FIDInfo::operator==(const FIDInfo &other) const {
    return hash_type_ == other.hash_type_ &&
           content_type_ == other.content_type_ &&
           base_type_ == other.base_type_;
};

bool FID::operator<(const FID &other) const {
    if (info_ < other.info_) {
        return true;
    } else if (info_ == other.info_) {
        if (hash_ < other.hash_) {
            return true;
        }
    }

    return false;
};

bool FID::operator==(const FID &other) const {
    return info_ == other.info_ && hash_ == other.hash_;
};

Path &File::getPath(const BlockDataHash &hash) {
    return blocks_[map_block_info[hash]].path_;
};

}   // namespace file_for_fs