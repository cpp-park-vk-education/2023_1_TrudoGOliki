#include "file_system.hpp"

namespace fs {

// insert can throw AVLT::AVLError
void AVLTreeSearch::insert(const F::FID &fid, F::File &&file) {
    try {
        insert(fid, std::move(file));
    } catch (AVLT::AVLError &e) {
        throw;
    }
};

void AVLTreeSearch::erase(const F::FID &fid) { erase(fid); };

F::File *AVLTreeSearch::find(const F::FID &fid) { return find(fid); };

void ManagerFilesCLI::createFile(const std::string &name,
                                 const F::Path &path_from, F::Path &path_to) {
    std::bitset<STANDARD_BLOCK_DATA_SIZE> cur_bitset;
}

}   // namespace fs