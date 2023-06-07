#pragma once

#include <utility>

#include "file.hpp"

#include "avl_tree_search.hpp"
#include "manager_files_cli.hpp"
#include "manager_files_net.hpp"

namespace fs {
class FileSystem {
  public:
    FileSystem(const std::string_view &name_main_dir,
               ITreeSearchFilesUP tree_search, IReaderUP reader,
               IWriterUP writer, IManagerCLIUP manager_cli);

    void selectNewReadFile(const file_fs::FID &fid);
    size_t getSizeFileRead() const;
    buf::Buffer getBuf();

    void createNewFileWrite(const file_fs::FID &fid,
                            const file_fs::FileInfo &info);
    void writeBuf(const buf::Buffer &buf);

    // addFile() can throw FSError
    file_fs::FID addFile(const file_fs::Path &path_from,
                         const std::string &description);

    // eraseFile() do nothing if file with fid not exist in FileSystem
    void eraseFile(const file_fs::FID &fid);

    // find() return nullptr if file with fid not exist
    file_fs::File *find(const file_fs::FID &fid);

    std::vector<file_fs::FID> getAllFids();
    std::vector<std::pair<file_fs::FID, file_fs::File>> getAll();

    ~FileSystem();

  private:
    ITreeSearchFilesUP tree_;
    ManagerFilesNet manager_net_;
    IManagerCLIUP manager_cli_;

    file_fs::Path path_main_dir_;

    void save();
    void recover();
    void createMainDir();
};
}   // namespace fs
