#pragma once

#include <bitset>
#include <filesystem>
#include <fstream>
#include <string_view>

#include "avl_tree.hpp"
#include "file.hpp"
#include "sha256.hpp"

namespace fs {
inline constexpr uint16_t STANDARD_BUFFER_SIZE = 4096;
inline constexpr std::string_view STANDART_HASH_TYPE = "sh";   // means sha256
inline constexpr std::string_view STANDART_CONTENT_TYPE =
    "tx";                                                      // means sha256
inline constexpr std::string_view STANDART_BASE_TYPE = "b6";   // means base64
inline constexpr std::string_view NAME_MAIN_DIR = ".main_dir";

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

struct Buffer {
    char *buf_;
    size_t size_;
};

class ManagerFilesNet {
  public:
    void selectNewFile(const F::Path &path);
    Buffer getBuf();

  private:
    std::ifstream in_;
};

class ManagerFilesCLI {
  public:
    ManagerFilesCLI(const F::Path &name_main_dir);

    void addFile(const F::Path &path);

  private:
    F::File *processed_file_;
    F::FID file_fid;
    F::Path path_main_dir_;

    void createMainDir();

    F::FID calculFID(const F::Path &path_from);

    void copyFile(const F::FID &fid, const F::Path &path_from);
};

class FileSystem {
  private:
    AVLTreeSearch tree;
    ManagerFilesCLI manager_cli;
};
}   // namespace fs
