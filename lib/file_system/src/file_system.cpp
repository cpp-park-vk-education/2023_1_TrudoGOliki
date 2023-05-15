#include "file_system.hpp"

namespace file_system {
void insert(FID &&fid, File &&file){};
void erase(const FID &fid){};
File *find(const FID &fid){};
}   // namespace file_system