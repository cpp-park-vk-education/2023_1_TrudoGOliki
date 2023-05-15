#pragma once

#include <optional>
#include <string>

struct Option {
    std::string_view flag;
    std::optional<std::string> argument;
};