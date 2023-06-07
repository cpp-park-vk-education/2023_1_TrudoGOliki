#include "avl_tree_search.hpp"

namespace fs {
// insert can throw AVLT::AVLError
void AVLTreeSearch::insert(const file_fs::FID &fid, file_fs::File &&file) {
    AVLTree::insert(fid, std::move(file));
};

void AVLTreeSearch::erase(const file_fs::FID &fid) { AVLTree::erase(fid); };

file_fs::File *AVLTreeSearch::find(const file_fs::FID &fid) {
    return AVLTree::find(fid);
};

std::vector<file_fs::FID> AVLTreeSearch::getAllFids() {
    return AVLTree::getAllKeys();
};

std::vector<std::pair<file_fs::FID, file_fs::File>> AVLTreeSearch::getAll() {
    return AVLTree::getAll();
};

}   // namespace fs
