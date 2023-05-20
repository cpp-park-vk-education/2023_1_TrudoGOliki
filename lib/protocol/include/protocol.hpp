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