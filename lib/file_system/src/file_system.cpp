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

void ManagerFilesNet::updateSize() { size_file_ = getSizeOfFile(f_stream_); };

// ManagerFilesNet::selectNewFileRead() can throw FSError
void ManagerFilesNet::selectNewFileRead(const F::Path &path) {
    if (f_stream_.is_open()) {
        f_stream_.close();
    }

    try {
        f_stream_.open(path.string(), std::ios::binary | std::ios::in);
        if (!f_stream_.is_open()) {
            throw FSError("file not opened");
        }
        updateSize();
    } catch (std::ios::failure &e) {
        throw FSError("in selectNewFileRead exception: " +
                      static_cast<std::string>(e.what()));
    }
}

size_t ManagerFilesNet::getSizeFileRead() const { return size_file_; }

// ManagerFilesNet::getBuf() can throw FSError and return buf::Buffer{buf_ ==
// nullptr and size_ = 0, when read of file done
buf::Buffer ManagerFilesNet::getBuf() {
    try {
        if (f_stream_.tellg() < size_file_) {
            buf::Buffer buffer = buf::Buffer(STANDARD_BUFFER_SIZE);
            f_stream_.read(buffer.buf_, STANDARD_BUFFER_SIZE);
            auto remainder = f_stream_.gcount();
            if (remainder == 0) {
                return buf::Buffer(0);
            }
            if (remainder < 0) {
                throw FSError("BLLL");
            }
            std::cout << "in read buf" << remainder << std::endl;
            buffer.size_ = static_cast<size_t>(remainder);
            return std::move(buffer);
        }
        return buf::Buffer(0);
    } catch (std::ios::failure &e) {
        throw FSError("in getBuf exception: " +
                      static_cast<std::string>(e.what()));
    }
}

// ManagerFileNet::createNewFileWrite() can throw FSError
void ManagerFilesNet::createNewFileWrite(const F::FID &fid,
                                         const F::File &file) {
    if (f_stream_.is_open()) {
        f_stream_.close();
    }

    size_file_ = file.info_.size_;
    std::string path_str = file.path_.string();
    // add insert in tree
    try {
        f_stream_.open(path_str, std::ios::binary | std::ios::out);
        if (!f_stream_.is_open()) {
            throw FSError("file not opened");
        }
    } catch (std::ios::failure &e) {
        throw FSError("in selectNewFileRead exception: " +
                      static_cast<std::string>(e.what()));
    }
}

void ManagerFilesNet::writeBuf(const buf::Buffer &buf) {
    if (f_stream_.tellp() < size_file_) {
        f_stream_.write(buf.buf_, buf.size_);
    } else {
        throw FSError("attempt write more than size of file ");
    }
}

F::FID ManagerFilesCLI::calculFID(const F::Path &path_from) {
    std::ifstream in(path_from.string());

    char buffer1[STANDARD_BUFFER_SIZE];
    char buffer2[STANDARD_BUFFER_SIZE];

    if (!in.is_open()) {
        throw FSError("in calculFID: can`t open file: " + path_from.string());
    }

    std::string prev_hash;
    while (true) {
        SHA256 sha256;
        bool flag_first_read = false;
        if (in.read(buffer1, STANDARD_BUFFER_SIZE)) {
            flag_first_read = true;
        };
        if (auto remainder = in.gcount(); remainder > 0) {
            std::string cur_hash;
            for (size_t i = 0; i < remainder; i++) {
                cur_hash += buffer1[i];
            }

            prev_hash +=
                sha256(prev_hash + sha256(cur_hash) + sha256(cur_hash));

            return F::FID{prev_hash};
        }

        bool flag_second_read = false;
        if (in.read(buffer2, STANDARD_BUFFER_SIZE)) {
            flag_second_read = true;
        };
        if (auto remainder = in.gcount(); remainder > 0) {
            std::string cur_hash;
            for (size_t i = 0; i < remainder; i++) {
                cur_hash += buffer2[i];
            }

            prev_hash +=
                sha256(prev_hash + sha256(buffer1, STANDARD_BUFFER_SIZE) +
                       sha256(cur_hash));
            return F::FID{prev_hash};
        }

        if (!flag_first_read && !flag_second_read) {
            return F::FID{prev_hash};
        }

        prev_hash = sha256(prev_hash + sha256(buffer1, STANDARD_BUFFER_SIZE) +
                           sha256(buffer2, STANDARD_BUFFER_SIZE));
    }
};

// ManagerFilesCLI::copyFile() can throw FSError
void ManagerFilesCLI::copyFile(const F::Path &path_from,
                               const F::Path &path_to) {
    std::ifstream in(path_from.string(), std::ios::binary);

    std::ofstream out(path_to.string(), std::ios::binary);

    char buffer[STANDARD_BUFFER_SIZE];

    if (!in.is_open() || !out.is_open()) {
        throw FSError("in copyFile: can`t open in or out");
    }

    while (true) {
        in.read(buffer, STANDARD_BUFFER_SIZE);
        auto remainder = in.gcount();
        if (remainder == 0) {
            break;
        }
        out.write(buffer, remainder);
    }
};

// addFile() can throw FSError
void ManagerFilesCLI::addFile(const F::Path &path_from,
                              const F::Path &path_to) {
    try {
        copyFile(path_from, path_to);
    } catch (FSError &e) {
        throw FSError("in addFile: from " + path_from.string() + " to " +
                      path_to.string() + static_cast<std::string>(e.what()));
    }
};

FileSystem::FileSystem(std::fstream &f_stream, const std::string &name_main_dir)
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
        auto path_to = path_main_dir_ / F::Path(fid.hash_);
        manager_cli_.addFile(path_from, path_to);
        auto file = new F::File();
        file->info_.description_ = description;

        std::fstream file_stream(path_to);
        size_t size = getSizeOfFile(file_stream);
        file->info_.size_ = size;
        file->path_ = path_to;
        tree_.insert(fid, std::move(*file));
        return fid;
    } catch (std::exception &e) {
        throw FSError("in FS::addFile:" + static_cast<std::string>(e.what()));
    }
}

void FileSystem::selectNewReadFile(const F::FID &fid) {
    F::File *file = tree_.find(fid);
    if (file) {
        manager_net_.selectNewFileRead(file->path_);
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
            throw FSError("file with FID: " + fid.string() + "already exist");
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

}   // namespace fs