#pragma once

#include <fstream>

#include "buffer.hpp"
#include "file.hpp"

namespace fs {
class ManagerFilesNet {
  public:
    ManagerFilesNet(std::fstream &f_stream) : f_stream_(f_stream){};

    void setSizeRead(size_t size);
    void selectNewFileRead(const file_fs::Path &path);
    buf::Buffer getBuf();
    size_t getSizeFileRead() const;

    void createNewFileWrite(const file_fs::FID &fid, const file_fs::File &file);
    void writeBuf(const buf::Buffer &buf);

  private:
    size_t size_file_;
    std::fstream &f_stream_;
};
}   // namespace fs
