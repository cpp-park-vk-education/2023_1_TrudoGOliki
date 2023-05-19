#include "file_system.hpp"

namespace fs {
// insert can throw AVLT::AVLError
void AVLTreeSearch::insert(const F::FID &fid, F::File &&file) {
    try {
        insert(fid, std::move(file));
    } catch (AVLT::AVLError &e) {
        throw;
    }
};

void AVLTreeSearch::erase(const F::FID &fid) { erase(fid); };

F::File *AVLTreeSearch::find(const F::FID &fid) { return find(fid); };

// ManagerFilesNet::selectNewFile() can throw FSError
void ManagerFilesNet::selectNewFile(const F::Path &path) {
    if (in_.is_open()) {
        in_.close();
    }

    in_ = std::ifstream(path.string(), std::ios::binary);
    if (!in_.is_open()) {
        throw FSError("in selectNewFile can`t open file");
    }
}

// ManagerFilesNet::getBuf() can throw FSError
Buffer ManagerFilesNet::getBuf() {
    char buffer[STANDARD_BUFFER_SIZE];
    if (in_.read(buffer, STANDARD_BUFFER_SIZE)) {
        return Buffer{buffer, STANDARD_BUFFER_SIZE};
    }
    if (in_.gcount() > 0) {
        return Buffer{buffer, static_cast<size_t>(in_.gcount())};
    }

    throw FSError("in getBuf can`t read file");
}

ManagerFilesCLI::ManagerFilesCLI(const F::Path &name_main_dir) {
    const char *homePath = std::getenv("HOME");
    path_main_dir_ = F::Path(homePath) / name_main_dir;
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
void ManagerFilesCLI::copyFile(const F::FID &fid, const F::Path &path_from) {
    std::ifstream in(path_from.string(), std::ios::binary);

    F::Path path_to_file = path_main_dir_ / fid.string();
    std::ofstream out(path_to_file.string(), std::ios::binary);

    char buffer[STANDARD_BUFFER_SIZE];

    if (!in.is_open() || !out.is_open()) {
        throw FSError("in copyFile: can`t open in or out");
    }

    while (in.read(buffer, STANDARD_BUFFER_SIZE)) {
        out.write(buffer, STANDARD_BUFFER_SIZE);
    }

    if (auto remainder = in.gcount(); remainder > 0) {
        for (size_t i = 0; i < remainder; i++) {
            out.write(buffer, remainder);
        }
    }
};

// addFile() can throw FSError
void ManagerFilesCLI::addFile(const F::Path &path_from) {
    try {
        createMainDir();
    } catch (std::exception &e) {
        throw FSError("in addFile: " + static_cast<std::string>(e.what()));
    }

    // calcul fid
    // copy file
};

// createMainDir() can throw FSError
void ManagerFilesCLI::createMainDir() {
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

}   // namespace fs