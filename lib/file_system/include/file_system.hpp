#pragma once

#include <avl_tree.hpp>
#include <file.hpp>

namespace file_system {
class ITreeSearchFiles {
  public:
    virtual void insert(FID &&fid, File &&file) = 0;
    virtual void erase(const FID &fid) = 0;
    virtual File *find(const FID &fid) = 0;
};

class TreeSearch : public ITreeSearchFiles,
                   public avl_tree::AVLTree<FID, File> {
    void insert(FID &&fid, File &&file) override;
    void erase(const FID &fid) override;
    File *find(const FID &fid) override;
}
}   // namespace file_system
