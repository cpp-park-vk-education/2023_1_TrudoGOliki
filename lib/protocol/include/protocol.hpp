#pragma once
#include <string_view>

class Protocol {
  public:
    Protocol() = default;

    void SendFile(std::string_view path, std::string_view ip);

  private:
};