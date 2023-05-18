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

ManagerFilesCLI::ManagerFilesCLI(const F::Path &name_main_dir) {
    const char *homePath = std::getenv("HOME");
    path_main_dir_ = F::Path(homePath) / name_main_dir;
}

// ManagerFilesCLI::copyFile() can throw FSError
void ManagerFilesCLI::copyFile(const F::FID &fid, const F::Path &path_from) {
    std::ifstream in(path_from.string());
    F::Path path_to_file = path_main_dir_ / fid.string();
    std::ofstream out(path_to_file.string());

    char buffer[STANDARD_BUFFER_SIZE];

    if (!in.is_open() || !out.is_open()) {
        throw FSError("in copyFile: can`t open in or out");
    }

    for (; in.read(buffer, STANDARD_BUFFER_SIZE);) {
        out << buffer;
    }

    if (in.gcount() < STANDARD_BUFFER_SIZE) {
        in.read(buffer, STANDARD_BUFFER_SIZE);
        out << buffer;
    }
};

// addFile() can throw FSError
void ManagerFilesCLI::addFile(const F::Path &path_from) {
    try {
        createMainDir();
    } catch (std::exception &e) {
        throw FSError("in addFile: " + static_cast<std::string>(e.what()));
    }
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