#include "file_system.hpp"

#include <fstream>
#include <iostream>

#include "fs_error.hpp"
#include "main_dir.hpp"

namespace fs {
static size_t getSizeOfFile(std::fstream &f_stream) {
    f_stream.seekg(0, std::ios::end);
    auto size = f_stream.tellg();
    f_stream.seekg(0, std::ios::beg);
    return static_cast<size_t>(size);
}

FileSystem::FileSystem(const std::string_view &name_main_dir,
                       ITreeSearchFilesUP tree_search, IReaderUP reader,
                       IWriterUP writer, IManagerCLIUP manager_cli)
    : manager_net_(ManagerFilesNet(std::move(reader), std::move(writer))),
      tree_(std::move(tree_search)), manager_cli_(std::move(manager_cli)) {
    const char *homePath = std::getenv("HOME");
    path_main_dir_ = file_fs::Path(homePath) / name_main_dir;

    try {
        recover();
    } catch (std::exception &e) {
        std::cout << "Recover failed because: " +
                         static_cast<std::string>(e.what())
                  << std::endl;
    }

    createMainDir();
}

// createMainDir() can throw FSError
void FileSystem::createMainDir() {
    if (!std::filesystem::exists(path_main_dir_)) {
        try {
            if (!std::filesystem::create_directory(path_main_dir_)) {
                throw FSError("in createMainDir: can`t create main directory");
            }
        } catch (std::exception &e) {
            throw FSError("in createMainDir: " +
                          static_cast<std::string>(e.what()));
        }
    }
};

file_fs::FID FileSystem::addFile(const file_fs::Path &path_from,
                                 const std::string &description) {

    file_fs::FID fid = manager_cli_->calculFID(path_from);
    file_fs::File *find_file = tree_->find(fid);
    if (find_file) {
        throw FSError(
            "in FS::addFile: file with same fid already exist. FID = " +
            fid.hash_);
    }

    auto path_to = path_main_dir_ / file_fs::Path(fid.hash_);
    manager_cli_->addFile(path_from, path_to);
    auto file = file_fs::File();
    file.info_.description_ = description;

    std::fstream file_stream(path_to);
    size_t size = getSizeOfFile(file_stream);
    file_stream.close();
    file.info_.size_ = size;
    file.path_ = path_to;
    try {
        tree_->insert(fid, std::move(file));
    } catch (avl_tree::AVLError &e) {
        throw FSError(
            "in FS::addFile: file with same fid already exist. FID = " +
            fid.hash_);
    }
    return fid;
}

// FileSystem::eraseFile() can throw FSError
void FileSystem::eraseFile(const file_fs::FID &fid) {
    try {
        auto file = tree_->find(fid);
        manager_cli_->eraseFile(file->path_);
        tree_->erase(fid);
    } catch (std::exception &e) {
        throw FSError("in eraseFile: can`t erase file with fid: " + fid.hash_ +
                      " because: " + static_cast<std::string>(e.what()));
    }
}

// FileSystem::selectNewReadFile() can throw FSError
void FileSystem::selectNewReadFile(const file_fs::FID &fid) {
    try {
        file_fs::File *file = tree_->find(fid);
        std::fstream check_size(file->path_);
        manager_net_.reader_->setSizeRead(getSizeOfFile(check_size));
        check_size.close();
        manager_net_.reader_->selectNewFileRead(file->path_);
    } catch (std::exception &e) {
        throw FSError(
            "in selectNewReadFile: can`t select file with fid: " + fid.hash_ +
            " because: " + static_cast<std::string>(e.what()));
    }
}

buf::Buffer FileSystem::getBuf() { return manager_net_.reader_->getBuf(); }

size_t FileSystem::getSizeFileRead() const {
    return manager_net_.reader_->getSizeFileRead();
};

// FileSystem::createNewFileWrite() can throw FSError
void FileSystem::createNewFileWrite(const file_fs::FID &fid,
                                    const file_fs::FileInfo &info) {
    try {
        file_fs::File *find_file = tree_->find(fid);
        auto file = file_fs::File();
        file.info_ = info;
        std::string extension = "";   // from info later
        file.path_ = path_main_dir_ / (fid.hash_ + extension);
        manager_net_.writer_->createNewFileWrite(fid, file);
        tree_->insert(fid, std::move(file));
    } catch (std::exception &e) {
        throw FSError(
            "in FileSystem::createNewFileWrite with file with fid = " +
            fid.hash_ + " because " + static_cast<std::string>(e.what()));
    }
}

void FileSystem::writeBuf(const buf::Buffer &buf) {
    return manager_net_.writer_->writeBuf(buf);
}

file_fs::File *FileSystem::find(const file_fs::FID &fid) {
    return tree_->find(fid);
}

std::vector<file_fs::FID> FileSystem::getAllFids() {
    return tree_->getAllFids();
};

std::vector<std::pair<file_fs::FID, file_fs::File>> FileSystem::getAll() {
    return tree_->getAll();
};

void FileSystem::save() {
    auto vec_all = getAll();
    auto size = vec_all.size();

    std::ofstream out(path_main_dir_ / MAIN_RECOVER_FILE, std::ios::binary);
    if (!out.is_open()) {
        throw FSError("in save fs: file MAIN_RECOVER not open");
    }

    out.write(reinterpret_cast<const char *>(&size), sizeof(size_t));

    for (int i = 0; i < size; i++) {
        buf::Buffer first = vec_all[i].first.serialize();
        buf::Buffer second = vec_all[i].second.serialize();
        out.write(first.get(), first.size_);
        out.write(second.get(), second.size_);
    }

    out.close();
    std::cout << "save file system succesfully";
};

template <typename T> static size_t getRemainder(T &file) {
    std::streampos begin = file.tellg();
    file.seekg(0, std::ios::end);
    size_t remainder = static_cast<size_t>(file.tellg() - begin);
    file.seekg(begin);

    return remainder;
};

void FileSystem::recover() {
    std::ifstream in(path_main_dir_ / MAIN_RECOVER_FILE, std::ios::binary);
    if (!in.is_open()) {
        throw FSError("in recover fs: file MAIN_RECOVER not open");
    }

    size_t size;
    in.read(reinterpret_cast<char *>(&size), sizeof(size_t));

    std::vector<std::pair<file_fs::FID, file_fs::File>> vec_all;
    vec_all.reserve(size);

    size_t size_buf = getRemainder(in);

    buf::Buffer buf(size_buf);
    in.read(buf.buf_.get(), size_buf);
    char *cur_position = buf.buf_.get();
    for (int i = 0; i < size; i++) {
        file_fs::FID fid;
        file_fs::File file;
        size_t size_fid = fid.deserialize(cur_position);
        cur_position += size_fid;
        size_t size_file = file.deserialize(cur_position);
        cur_position += size_file;

        vec_all.push_back({fid, file});
    }

    for (auto el : vec_all) {
        tree_->insert(el.first, std::move(el.second));
    }

    std::cout << "recover filesystem succesfully";
};

FileSystem::~FileSystem() {
    try {
        save();
    } catch (std::exception &e) {
        std::cout << "Save file system failed because: " +
                         static_cast<std::string>(e.what())
                  << std::endl;
    }
};

}   // namespace fs