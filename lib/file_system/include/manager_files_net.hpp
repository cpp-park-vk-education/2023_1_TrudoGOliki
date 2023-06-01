#pragma once

#include <fstream>

#include "buffer.hpp"
#include "file.hpp"

namespace fs {
class WriterNet {
  public:
    void createNewFileWrite(const file_fs::FID &fid, const file_fs::File &file);
    void writeBuf(const buf::Buffer &buf);

  private:
    size_t size_file_;
    std::ofstream of_stream_;
};

class ReaderNet {
  public:
    void selectNewFileRead(const file_fs::Path &path);
    buf::Buffer getBuf();
    void setSizeRead(size_t size);
    size_t getSizeFileRead() const;

  private:
    size_t size_file_;
    std::fstream f_stream_;
};

class ManagerFilesNet {
  public:
    ReaderNet reader_;
    WriterNet writer_;
};
}   // namespace fs
