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

  private:
    void copyBuf(const char *from_buf, char *to_buf, size_t count);
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

auto STANDARD_BUFFER_SIZE = 1024;

using Path = std::filesystem::path;

struct FID {
    std::string hash_;

    bool operator<(const FID &other) const;
    bool operator==(const FID &other) const;
    std::string string() const;

    Buffer serialize();
    void deserialize(const Buffer &buf);
};

bool FID::operator<(const FID &other) const {
    if (hash_ < other.hash_) {
        return true;
    }

    return false;
};

bool FID::operator==(const FID &other) const { return hash_ == other.hash_; };

std::string FID::string() const { return hash_; };

Buffer FID::serialize() { return Buffer(hash_.c_str(), hash_.size()); };

void FID::deserialize(const Buffer &buf) {
    for (int i = 0; i < buf.size_; i++) {
        hash_ += buf.buf_[i];
    }
};

struct FileInfo {
    FileInfo();
    FileInfo(std::string description, size_t size);
    std::string description_;
    size_t size_;
};

class File {
  public:
    Path path_;
    FileInfo info_;

    Buffer serialize();
    void deserialize(const Buffer &buf);
};

FileInfo::FileInfo() : size_(0){};
FileInfo::FileInfo(std::string description, size_t size)
    : description_(std::move(description)), size_(size){};

Buffer File::serialize() {
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

void File::deserialize(const Buffer &buf) {
    size_t path_size;
    char *cur_position;
    std::memcpy(&path_size, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    std::string path_str(cur_position, path_size);
    cur_position += path_size;
    path_ = Path(path_str);

    size_t desc_size;
    std::memcpy(&desc_size, cur_position, sizeof(size_t));
    cur_position += sizeof(size_t);
    info_.description_ = std::string(cur_position, path_size);
    cur_position += desc_size;

    std::memcpy(&info_.size_, cur_position, sizeof(size_t));
};

constexpr char *MainFile = "main_file";

int main() {
    std::ofstream out(MainFile, std::ios::binary);
    std::vector<std::pair<FID, File>> vec;
    std::string str = "./aaaa";
    for (int i = 0; i < 10; i++) {
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
        out.write(reinterpret_cast<const char *>(&first.buf_), first.size_);
        out.write(reinterpret_cast<const char *>(&second.buf_), second.size_);
    }

    out.close();
    std::ifstream in(MainFile, std::ios::binary);

    int size_f;
    in.read(reinterpret_cast<char *>(&size_f), sizeof(size_f));

    std::vector<std::pair<FID, File>> vec_n;
    vec_n.reserve(size_f);
    for (int i = 0; i < size_f; i++) {
        // in.read(reinterpret_cast<char *>(&fid), sizeof(FID));
        // in.read(reinterpret_cast<char *>(&file), sizeof(File));
        // vec_n.push_back(std::pair<FID, File>(fid, file));
    }
    std::cout << "end";
    return 0;
}