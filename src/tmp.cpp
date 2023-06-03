#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

struct FIDInfo {
    const std::string hash_type_;
    const std::string content_type_;
    const std::string base_type_;
};

void copyBuf(const char *from_buf, char *to_buf, size_t count) {
    for (int i = 0; i < count; i++) {
        to_buf[i] = from_buf[i];
    }
};

class Buffer {
  public:
    explicit Buffer(size_t size);
    Buffer(const char *buf, size_t size);
    Buffer(const Buffer &other)
        : buf_(new char[other.size_]), size_(other.size_) {
        copyBuf(other.buf_, this->buf_, other.size_);
    };
    Buffer(Buffer &&other);

    Buffer &operator=(const Buffer &other);
    Buffer &operator=(Buffer &&other);

    ~Buffer();

    char *buf_;
    size_t size_;
};

// Buffer(size_t size) can throw std::runtime_error if size==0
Buffer::Buffer(size_t size) : size_(size) {
    if (size == 0) {
        throw std::runtime_error("can`t create buffer with size=0");
    } else {
        buf_ = new char[size];
    }
};

// Buffer(char *buf, size_t size) can throw std::runtime_error if size==0 or
// buf==nullptr
Buffer::Buffer(const char *buf, size_t size) {
    if (buf != nullptr && size != 0) {
        copyBuf(buf, buf_, size);
    }
    throw std::runtime_error("can`t create buffer with size=" +
                             std::to_string(size) + " and buf=" + buf);
}

Buffer::Buffer(Buffer &&other) {
    size_ = other.size_;
    buf_ = std::exchange(other.buf_, nullptr);
};

Buffer &Buffer::operator=(Buffer &&other) {
    if (this != &other) {
        this->~Buffer();
        size_ = other.size_;
        buf_ = std::exchange(other.buf_, nullptr);
    }

    return *this;
};

Buffer &Buffer::operator=(const Buffer &other) {
    if (this != &other) {
        char *tmp = new char[other.size_];
        copyBuf(other.buf_, tmp, other.size_);
        delete[] buf_;

        buf_ = tmp;
        size_ = other.size_;
    }
    return *this;
};

Buffer::~Buffer() { delete[] buf_; }

class ISerializable {
  public:
    virtual Buffer serialize() const = 0;

    // deserialize() return count bytes used in deserialize
    virtual size_t deserialize(const char *buf) = 0;

    virtual ~ISerializable() = default;
};

auto STANDARD_BUFFER_SIZE = 1024;

using Path = std::filesystem::path;

struct FID : public ISerializable {
    FID(){};
    FID(std::string hash);
    std::string hash_;

    bool operator<(const FID &other) const;
    bool operator==(const FID &other) const;
    std::string string() const;

    Buffer serialize() const override;
    size_t deserialize(const char *buf) override;
};

FID::FID(std::string hash) : hash_(std::move(hash)){};

bool FID::operator<(const FID &other) const {
    if (hash_ < other.hash_) {
        return true;
    }

    return false;
};

bool FID::operator==(const FID &other) const { return hash_ == other.hash_; };

std::string FID::string() const { return hash_; };

Buffer FID::serialize() const {
    size_t size_hash = hash_.size();
    Buffer buf = Buffer(size_hash + sizeof(size_t));
    char *cur_positon = buf.buf_;

    std::memcpy(cur_positon, &size_hash, sizeof(size_t));
    cur_positon += sizeof(size_t);
    copyBuf(hash_.c_str(), cur_positon, size_hash);
    cur_positon += size_hash;

    return buf;
};

size_t FID::deserialize(const char *buf) {
    size_t size_hash;
    const char *cur_position = buf;
    std::memcpy(&size_hash, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    hash_ = std::string(cur_position, size_hash);
    cur_position += size_hash;

    return cur_position - buf;
};

struct FileInfo {
    FileInfo();
    FileInfo(std::string description, size_t size);
    std::string description_;
    size_t size_;
};

class File : public ISerializable {
  public:
    File(){};
    File(Path path, FileInfo info);
    Path path_;
    FileInfo info_;

    Buffer serialize() const override;
    size_t deserialize(const char *buf) override;
};

File::File(Path path, FileInfo info)
    : path_(std::move(path)), info_(std::move(info)){};

FileInfo::FileInfo() : size_(0){};
FileInfo::FileInfo(std::string description, size_t size)
    : description_(std::move(description)), size_(size){};

Buffer File::serialize() const {
    std::string path_str = path_.string();
    size_t path_size = path_str.size();

    std::string description = info_.description_;
    size_t desc_size = description.size();

    Buffer buf = Buffer(desc_size + path_size + 3 * sizeof(size_t));
    char *cur_positon = buf.buf_;

    std::memcpy(cur_positon, &path_size, sizeof(size_t));
    cur_positon += sizeof(size_t);
    copyBuf(path_str.c_str(), cur_positon, path_size);
    cur_positon += path_size;

    std::memcpy(cur_positon, &desc_size, sizeof(size_t));
    cur_positon += sizeof(size_t);
    copyBuf(description.c_str(), cur_positon, desc_size);
    cur_positon += desc_size;

    std::memcpy(cur_positon, &info_.size_, sizeof(size_t));

    return buf;
};

size_t File::deserialize(const char *buf) {
    size_t path_size;
    const char *cur_position = buf;
    std::memcpy(&path_size, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    std::string path_str(cur_position, path_size);
    cur_position += path_size;
    path_ = Path(path_str);

    size_t desc_size;
    std::memcpy(&desc_size, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    info_.description_ = std::string(cur_position, desc_size);
    cur_position += desc_size;

    std::memcpy(&info_.size_, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);

    return buf - cur_position;
};

constexpr std::string_view MainFile = "main_file";

int main() {
    std::ofstream out(std::string(MainFile), std::ios::binary);
    std::vector<std::pair<FID, File>> vec;
    std::string str = "./aaaa";
    for (int i = 0; i < 1; i++) {
        vec.push_back({FID{str + std::to_string(rand() % 10)},
                       File{Path{str + std::to_string(rand() % 10)},
                            FileInfo{str + std::to_string(rand() % 10), 10}}});
        std::cout << sizeof(vec[i]) << std::endl;
    }

    auto size = vec.size();
    out.write(reinterpret_cast<const char *>(&size), sizeof(size));

    for (int i = 0; i < size; i++) {
        Buffer first = vec[i].first.serialize();
        Buffer second = vec[i].second.serialize();
        out.write(first.buf_, first.size_);
        out.write(second.buf_, second.size_);
    }

    out.close();
    std::ifstream in(std::string(MainFile), std::ios::binary);

    size_t size_f;
    in.read(reinterpret_cast<char *>(&size_f), sizeof(size_t));

    std::vector<std::pair<FID, File>> vec_n;
    vec_n.reserve(size_f);
    std::streampos begin = in.tellg();
    in.seekg(0, std::ios::end);
    size_t size_buf = static_cast<size_t>(in.tellg() - begin);
    in.seekg(begin);

    Buffer buf(size_buf);
    in.read(buf.buf_, size_buf);
    char *cur_position = buf.buf_;
    for (int i = 0; i < size_f; i++) {
        FID fid;
        File file;
        size_t size_fid = fid.deserialize(cur_position);
        cur_position += size_fid;
        size_t size_file = file.deserialize(cur_position);
        cur_position += size_file;

        vec_n.push_back({fid, file});
    }
    std::cout << "end";
    return 0;
}