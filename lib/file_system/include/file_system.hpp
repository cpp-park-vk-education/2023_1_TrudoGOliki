#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include "avl_tree.hpp"
#include "buffer.hpp"
#include "file.hpp"
#include "sha256.hpp"

namespace fs {
inline constexpr uint16_t STANDARD_BUFFER_SIZE = 4096;
inline constexpr std::string_view NAME_MAIN_DIR =
    "techno_park/2023_1_TrudoGOliki/build";

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

class ManagerFilesNet {
  public:
    ManagerFilesNet(std::fstream &f_stream) : f_stream_(f_stream){};

    void selectNewFileRead(const F::Path &path);
    buf::Buffer getBuf();
    size_t getSizeFileRead() const;

    void createNewFileWrite(const F::FID &fid, const F::File &file);
    void writeBuf(const buf::Buffer &buf);

  private:
    size_t size_file_;
    std::fstream &f_stream_;

    void updateSize();
};

class ManagerFilesCLI {
  public:
    void addFile(const F::Path &path_from, const F::Path &path_to);

    F::FID calculFID(const F::Path &path_from);

    void eraseFile(const F::Path &path_to);

  private:
    void copyFile(const F::Path &path_from, const F::Path &path_to);
};

class FileSystem {
  public:
    FileSystem(std::fstream &f_stream, const std::string_view &name_main_dir);
    void selectNewReadFile(const F::FID &fid);
    buf::Buffer getBuf();
    size_t getSizeFileRead() const;

    void createNewFileWrite(const F::FID &fid, const F::FileInfo &info);
    void writeBuf(const buf::Buffer &buf);

    F::FID addFile(const F::Path &path_from, const std::string &description);

    void eraseFile(const F::FID &fid);

  private:
    AVLTreeSearch tree_;
    ManagerFilesNet manager_net_;
    ManagerFilesCLI manager_cli_;

    F::Path path_main_dir_;

    void createMainDir();
};
}   // namespace fs
