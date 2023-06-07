#pragma once
#include <string_view>

namespace fs {
inline constexpr uint16_t STANDARD_BUFFER_SIZE = 1024;
inline constexpr std::string_view NAME_MAIN_DIR = ".main_dir";
constexpr std::string_view MAIN_RECOVER_FILE = ".main_recover_file";
}