#pragma once

#include "IExecutable.hpp"
#include "Option.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

using cmdT = std::string;

class CLI {
  public:
    CLI() {}
    void Execute();
    void ParseArgs(int argc, char *argv[]);

  private:
    std::vector<std::string_view> argv;

    std::vector<Option> options;
    cmdT command;

    std::map<cmdT, std::unique_ptr<IExecutable>> handler;
};

/*
struct Arguments {
    bool help{false};
    std::string value;
};

Arguments parse_args(int argc, const char *argv[]);*/