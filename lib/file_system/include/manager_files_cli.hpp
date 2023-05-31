#include "file.hpp"

namespace fs {
class ManagerFilesCLI {
  public:
    void addFile(const file_fs::Path &path_from, const file_fs::Path &path_to);

    file_fs::FID calculFID(const file_fs::Path &path_from);

    void eraseFile(const file_fs::Path &path_to);

  private:
    void copyFile(const file_fs::Path &path_from, const file_fs::Path &path_to);
};

}   // namespace fs
