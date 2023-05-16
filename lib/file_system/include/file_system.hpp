#pragma once

#include <avl_tree.hpp>
#include <bitset>
#include <file.hpp>
#include <filesystem>

namespace fs {
inline constexpr int STANDARD_BLOCK_DATA_SIZE = 1024 * 1024;

class FSError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

namespace F = file_for_fs;
namespace AVLT = avl_tree;

class ITreeSearchFiles {
  public:
    virtual void insert(const F::FID &fid, F::File &&file) = 0;
    virtual void erase(const F::FID &fid) = 0;
    virtual F::File *find(const F::FID &fid) = 0;
};

class AVLTreeSearch : public ITreeSearchFiles,
                      public AVLT::AVLTree<F::FID, F::File> {
    void insert(const F::FID &fid, F::File &&file) override;
    void erase(const F::FID &fid) override;
    F::File *find(const F::FID &fid) override;
};

class ManagerFilesCLI {
  public:
    void createDir(const std::string &name, const F::Path &path);
    void createFile(const std::string &name, const F::Path &path_from,
                    F::Path &path_to);

  private:
    F::File *processed_file_;
    F::FID file_fid;
};

class FileSystem {
  private:
    AVLTreeSearch tree;
    ManagerFilesCLI manager_cli;
};
}   // namespace fs
