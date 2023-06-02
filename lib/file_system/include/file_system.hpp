#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>

#include "avl_tree.hpp"
#include "buffer.hpp"
#include "constants.hpp"
#include "errors.hpp"
#include "file.hpp"
#include "manager_files_cli.hpp"
#include "manager_files_net.hpp"
#include "sha256.hpp"

namespace fs {
namespace F = file_fs;
namespace AVLT = avl_tree;

class ITreeSearchFiles;
class IManagerCLI;
using ITreeSearchFilesUP = std::unique_ptr<ITreeSearchFiles>;
using IManagerCLIUP = std::unique_ptr<IManagerCLI>;

class ITreeSearchFiles {
  public:
    virtual void insert(const F::FID &fid, F::File &&file) = 0;
    virtual void erase(const F::FID &fid) = 0;
    virtual F::File *find(const F::FID &fid) = 0;

    virtual std::vector<F::FID> getAllFids() = 0;
    virtual std::vector<std::pair<F::FID, F::FileInfo>> getAll() = 0;

    virtual ~ITreeSearchFiles() = default;
};

class AVLTreeSearch : public AVLT::AVLTree<F::FID, F::File>,
                      public ITreeSearchFiles {
  public:
    void insert(const F::FID &fid, F::File &&file) override;
    void erase(const F::FID &fid) override;
    F::File *find(const F::FID &fid) override;

    std::vector<F::FID> getAllFids() override;
    std::vector<std::pair<F::FID, F::FileInfo>> getAll() override;
};

class FileSystem {
  public:
    FileSystem(const std::string_view &name_main_dir,
               ITreeSearchFilesUP tree_search, IReaderUP reader,
               IWriterUP writer, IManagerCLIUP manager_cli);

    void selectNewReadFile(const F::FID &fid);
    buf::Buffer getBuf();
    size_t getSizeFileRead() const;

    void createNewFileWrite(const F::FID &fid, const F::FileInfo &info);
    void writeBuf(const buf::Buffer &buf);

    F::FID addFile(const F::Path &path_from, const std::string &description);

    // eraseFile() do nothing if file with fid not exist in FileSystem
    void eraseFile(const F::FID &fid);

    // find() return nullptr if file with fid not exist
    F::File *find(const F::FID &fid);

    std::vector<F::FID> getAllFids();
    std::vector<std::pair<F::FID, F::FileInfo>> getAll();

  private:
    ITreeSearchFilesUP tree_;
    ManagerFilesNet manager_net_;
    IManagerCLIUP manager_cli_;

    F::Path path_main_dir_;

    void createMainDir();
};
}   // namespace fs
