#pragma once

#include "file_system.hpp"
#include <string_view>

class Protocol {
  public:
    Protocol() = default;

    void SendFile(std::string_view path, std::string_view ip);
    void ReciveFile(int fd, fs::FileSystem &f_s);

  private:
};

using len_file_t = size_t;
using type_file_t = std::string;
using fid_t = std::string;

struct Data {
    len_file_t len_file;
    type_file_t type_file;
    fid_t fid;

    buf::Buffer serialize();
    Data(len_file_t len, type_file_t type, fid_t fid)
        : len_file(len), type_file(type), fid(fid){};
    Data(buf::Buffer);

    Data *deserialize(char *bytes) { return reinterpret_cast<Data *>(bytes); }

    void operator=(const Data *other) {
        this->len_file = other->len_file;
        this->type_file = other->type_file;
        this->fid = other->fid;
    }
};

template <typename T>

char *serialize(T *object) {
    return reinterpret_cast<char *>(object);
}

template <typename T>

T *deserialize(char *bytes) {
    return reinterpret_cast<T *>(bytes);
}