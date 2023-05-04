#pragma once

#include <string>

struct Arguments {
    bool help{false};
    std::string value;
};

Arguments parse_args(int argc, const char *argv[]);