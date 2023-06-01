#pragma once

#include <fstream>
#include <memory>

#include "buffer.hpp"
#include "file.hpp"

namespace fs {
class IWriterNet {
  public:
    virtual void createNewFileWrite(const file_fs::FID &fid,
                                    const file_fs::File &file) = 0;
    virtual void writeBuf(const buf::Buffer &buf) = 0;
    virtual ~IWriterNet() = 0;
};

class WriterNet : public IWriterNet {
  public:
    void createNewFileWrite(const file_fs::FID &fid,
                            const file_fs::File &file) override;
    void writeBuf(const buf::Buffer &buf) override;

  private:
    size_t size_file_;
    std::ofstream of_stream_;
};

class IReaderNet {
  public:
    virtual void selectNewFileRead(const file_fs::Path &path) = 0;
    virtual buf::Buffer getBuf() = 0;
    virtual void setSizeRead(size_t size) = 0;
    virtual size_t getSizeFileRead() const = 0;
    virtual ~IReaderNet() = 0;
};

class ReaderNet : public IReaderNet {
  public:
    ReaderNet();
    void selectNewFileRead(const file_fs::Path &path) override;
    buf::Buffer getBuf() override;
    void setSizeRead(size_t size) override;
    size_t getSizeFileRead() const override;
    ~ReaderNet() override;

  private:
    size_t size_file_;
    std::fstream if_stream_;
};

class ManagerFilesNet {
  public:
    ManagerFilesNet();
    std::unique_ptr<IReaderNet> reader_;
    std::unique_ptr<WriterNet> writer_;
};
}   // namespace fs
