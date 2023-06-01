#include "file_system.hpp"

namespace fs {
static size_t getSizeOfFile(std::fstream &f_stream) {
    f_stream.seekg(0, std::ios::end);
    auto size = f_stream.tellg();
    f_stream.seekg(0, std::ios::beg);
    return static_cast<size_t>(size);
}

// insert can throw AVLT::AVLError
void AVLTreeSearch::insert(const F::FID &fid, F::File &&file) {
    AVLTree::insert(fid, std::move(file));
};

void AVLTreeSearch::erase(const F::FID &fid) { AVLTree::erase(fid); };

F::File *AVLTreeSearch::find(const F::FID &fid) { return AVLTree::find(fid); };

// FileSystem::eraseFile() can throw FSError
void FileSystem::eraseFile(const F::FID &fid) {
    auto file = tree_.find(fid);
    if (file) {
        manager_cli_.eraseFile(file->path_);
        tree_.erase(fid);
    } else {
        throw FSError("in eraseFile: can`t erase file with fid: " +
                      fid.string());
    }
}

FileSystem::FileSystem(std::fstream &f_stream,
                       const std::string_view &name_main_dir)
    : manager_net_(f_stream) {
    const char *homePath = std::getenv("HOME");
    path_main_dir_ = F::Path(homePath) / name_main_dir;
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

F::FID FileSystem::addFile(const F::Path &path_from,
                           const std::string &description) {
    try {
        F::FID fid = manager_cli_.calculFID(path_from);
        F::File *find_file = tree_.find(fid);
        if (find_file) {
            throw FSError("file with FID: " + fid.string() + " already exist");
        }

        auto path_to = path_main_dir_ / F::Path(fid.hash_);
        manager_cli_.addFile(path_from, path_to);
        auto file = new F::File();
        file->info_.description_ = description;

        std::fstream file_stream(path_to);
        size_t size = getSizeOfFile(file_stream);
        file_stream.close();
        file->info_.size_ = size;
        file->path_ = path_to;
        tree_.insert(fid, std::move(*file));
        return fid;
    } catch (std::exception &e) {
        throw FSError("in FS::addFile:" + static_cast<std::string>(e.what()));
    }
}

// FileSystem::selectNewReadFile() can throw FSError
void FileSystem::selectNewReadFile(const F::FID &fid) {
    F::File *file = tree_.find(fid);
    if (file) {
        std::fstream check_size(file->path_);
        manager_net_.setSizeRead(getSizeOfFile(check_size));
        check_size.close();
        manager_net_.selectNewFileRead(file->path_);
    } else {
        throw FSError("in selectNewReadFile: can`t select file with fid: " +
                      fid.string());
    }
}

buf::Buffer FileSystem::getBuf() { return manager_net_.getBuf(); }

size_t FileSystem::getSizeFileRead() const {
    return manager_net_.getSizeFileRead();
};

// FileSystem::createNewFileWrite() can throw FSError
void FileSystem::createNewFileWrite(const F::FID &fid,
                                    const F::FileInfo &info) {
    try {
        F::File *find_file = tree_.find(fid);
        if (find_file) {
            throw FSError("file with FID: " + fid.string() + " already exist");
        }
        auto file = new F::File();
        file->info_ = info;
        std::string extension = "";   // from info later
        file->path_ = path_main_dir_ / (fid.string() + extension);
        manager_net_.createNewFileWrite(fid, *file);
        tree_.insert(fid, std::move(*file));
    } catch (std::exception &e) {
        throw FSError("in FileSystem::createNewFileWrite: " +
                      static_cast<std::string>(e.what()));
    }
}

void FileSystem::writeBuf(const buf::Buffer &buf) {
    return manager_net_.writeBuf(buf);
}

F::File *FileSystem::find(const F::FID &fid) { return tree_.find(fid); }

}   // namespace fs