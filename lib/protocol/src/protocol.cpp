#include "protocol.hpp"
#include "connection.hpp"

#include <iostream>

buf::Buffer Data::serialize() {

    if (type_file.length() > 255)
        throw std::runtime_error("type file _field_ byte size is too big");

    if (fid.length() > 255)
        throw std::runtime_error("fid _field_ byte size is too big");

    auto buf =
        buf::Buffer(sizeof(len_file) + type_file.length() + fid.length() + 2);
    size_t ind = 0;
    for (size_t i = 0; i < sizeof(len_file); ++i) {
        buf.buf_[ind++] = *((char *) &len_file + i);
    }

    buf.buf_[ind++] = type_file.length();
    for (size_t i = 0; i < type_file.length(); ++i) {
        buf.buf_[ind++] = type_file[i];
    }

    buf.buf_[ind++] = fid.length();
    for (size_t i = 0; i < fid.length(); ++i) {
        buf.buf_[ind++] = fid[i];
    }
    return buf;
}

Data::Data(buf::Buffer buf) {
    size_t ind = 0;
    for (size_t i = 0; i < sizeof(len_file); ++i) {
        *((char *) &len_file + i) = buf.buf_[ind++];
    }

    type_file.resize(buf.buf_[ind++]);
    for (size_t i = 0; i < type_file.length(); ++i) {
        type_file[i] = buf.buf_[ind++];
    }

    fid.resize(buf.buf_[ind++]);
    for (size_t i = 0; i < fid.length(); ++i) {
        fid[i] = buf.buf_[ind++];
    }
}
