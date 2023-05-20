#pragma once

#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include "avl_tree.hpp"
#include "file.hpp"
#include "sha256.hpp"

namespace fs {
inline constexpr uint16_t STANDARD_BUFFER_SIZE = 4096;
inline constexpr std::string_view NAME_MAIN_DIR = ".main_dir";

class FSError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

namespace F = file_for_fs;
namespace AVLT = avl_tree;

// class ITreeSearchFiles {
//   public:
//     virtual void insert(const F::FID &fid, F::File &&file) = 0;
//     virtual void erase(const F::FID &fid) = 0;
//     virtual F::File *find(const F::FID &fid) = 0;
// };

class AVLTreeSearch : public AVLT::AVLTree<F::FID, F::File> {
  public:
    void insert(const F::FID &fid, F::File &&file);
    void erase(const F::FID &fid);
    F::File *find(const F::FID &fid);
};

class MockTree {
  public:
    void insert(const F::FID &fid, F::File &&file){};
    F::File *find(const F::FID &fid) {
        return new F::File{
            "/home/vilin/Documents/grow/docker_na_praktike_15-50.pdf",
            {"sdf", 10}};
    }
};

struct Buffer {
    char *buf_;
    size_t size_;
};

class ManagerFilesNet {
  public:
    ManagerFilesNet(std::fstream &f_stream) : f_stream_(f_stream){};

    void selectNewFileRead(const F::Path &path);
    Buffer getBuf();
    size_t getSizeFileRead() const;

  private:
    size_t size_file_;
    std::fstream &f_stream_;

    void updateSize();
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
  public:
    FileSystem(std::fstream &f_stream);
    void selectNewReadFile(const F::FID &fid);
    Buffer getBuf() { return manager_net_.getBuf(); }
    size_t getSizeFileRead() const { return manager_net_.getSizeFileRead(); };

  private:
    MockTree tree_;
    // ManagerFilesCLI manager_cli_;
    ManagerFilesNet manager_net_;
};
}   // namespace fs
