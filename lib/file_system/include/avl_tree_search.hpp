#pragma once

#include <memory>

#include "avl_tree.hpp"
#include "file.hpp"

namespace fs {
namespace AVLT = avl_tree;

class ITreeSearchFiles;
using ITreeSearchFilesUP = std::unique_ptr<ITreeSearchFiles>;

class ITreeSearchFiles {
  public:
    virtual void insert(const file_fs::FID &fid, file_fs::File &&file) = 0;
    virtual void erase(const file_fs::FID &fid) = 0;
    virtual file_fs::File *find(const file_fs::FID &fid) = 0;

    virtual std::vector<file_fs::FID> getAllFids() = 0;
    virtual std::vector<std::pair<file_fs::FID, file_fs::File>> getAll() = 0;

    virtual ~ITreeSearchFiles() = default;
};

class AVLTreeSearch : public AVLT::AVLTree<file_fs::FID, file_fs::File>,
                      public ITreeSearchFiles {
  public:
    void insert(const file_fs::FID &fid, file_fs::File &&file) override;
    void erase(const file_fs::FID &fid) override;
    file_fs::File *find(const file_fs::FID &fid) override;

    std::vector<file_fs::FID> getAllFids() override;
    std::vector<std::pair<file_fs::FID, file_fs::File>> getAll() override;
};

}   // namespace fs
