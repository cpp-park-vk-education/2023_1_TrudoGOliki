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
inline constexpr uint16_t STANDARD_BUFFER_SIZE = 1024;
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
            "/home/vilin/techno_park/2023_1_TrudoGOliki/build/yazyk_go.pdf",
            {"sdf", 10}};
    }
};

struct Buffer {
    explicit Buffer(size_t size) : size_(size) {
        if (size == 0) {
            buf_ = nullptr;
        } else {
            buf_ = new char[size];
        }
    };
    Buffer(const Buffer &other)
        : buf_(new char[other.size_]), size_(other.size_) {
        copyBuf(other.buf_, this->buf_, other.size_);
    };
    Buffer &operator=(const Buffer &other) {
        if (this != &other) {
            char *tmp = new char[other.size_];
            copyBuf(other.buf_, tmp, other.size_);
            delete[] buf_;

            buf_ = tmp;
            size_ = other.size_;
        }
        return *this;
    };
    ~Buffer() { delete[] buf_; }

    void copyBuf(char *buf1, char *buf2, size_t count) {
        for (int i = 0; i < count; i++) {
            buf2[i] = buf1[i];
        }
    };
    char *buf_;
    size_t size_;
};

class ManagerFilesNet {
  public:
    ManagerFilesNet(std::fstream &f_stream) : f_stream_(f_stream){};

    void selectNewFileRead(const F::Path &path);
    Buffer getBuf();
    size_t getSizeFileRead() const;

    void createNewFileWrite(const F::FID &fid, const F::File &file);
    void writeBuf(const Buffer &buf);

  private:
    size_t size_file_;
    std::fstream &f_stream_;

    void updateSize();
};

class ManagerFilesCLI {
  public:
    void addFile(const F::Path &path);

    void copyFile(const F::FID &fid, const F::Path &path_from,
                  const F::Path &path_to);

  private:
    F::File *processed_file_;
    F::FID file_fid;

    F::FID calculFID(const F::Path &path_from);
};

class FileSystem {
  public:
    FileSystem(std::fstream &f_stream, const std::string &name_main_dir);
    void selectNewReadFile(const F::FID &fid);
    Buffer getBuf();
    size_t getSizeFileRead() const;

    void createNewFileWrite(const F::FID &fid, const F::FileInfo &info);
    void writeBuf(const Buffer &buf);

    ManagerFilesCLI manager_cli_;

  private:
    MockTree tree_;
    AVLTreeSearch tree2_;
    ManagerFilesNet manager_net_;

    F::Path path_main_dir_;

    void createMainDir();
};
}   // namespace fs
