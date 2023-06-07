#pragma once

#include "file.hpp"

namespace fs {
class IManagerCLI;
using IManagerCLIUP = std::unique_ptr<IManagerCLI>;

class IManagerCLI {
  public:
    virtual void addFile(const file_fs::Path &path_from,
                         const file_fs::Path &path_to) = 0;

    virtual file_fs::FID calculFID(const file_fs::Path &path_from) = 0;

    virtual void eraseFile(const file_fs::Path &path_to) = 0;
    virtual ~IManagerCLI() = default;
};

class ManagerFilesCLI : public IManagerCLI {
  public:
    void addFile(const file_fs::Path &path_from,
                 const file_fs::Path &path_to) override;

    file_fs::FID calculFID(const file_fs::Path &path_from) override;

    void eraseFile(const file_fs::Path &path_to) override;

  private:
    void copyFile(const file_fs::Path &path_from, const file_fs::Path &path_to);
};

}   // namespace fs
