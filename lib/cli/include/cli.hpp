#pragma once

#include "IExecutable.hpp"
#include "Option.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class CLI {
  public:
    CLI();
    void Execute();
    void ParseArgs(int argc, char *argv[]);

    class CliParseError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

  private:
    std::vector<Option> options;
    Command cmd;

    handler<IExecutablePtr> CmdHandler;
    checker Opt0Set;
    checker Opt1Set;
    checker Cmd0Set;
    checker Cmd1Set;
};

/*
struct Arguments {
    bool help{false};
    std::string value;
};

Arguments parse_args(int argc, const char *argv[]);*/