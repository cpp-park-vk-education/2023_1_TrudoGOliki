#include "cli.hpp"

#include <functional>
#include <iostream>

using Args0Func = std::function<void(Arguments &)>;

const std::unordered_map<std::string, Args0Func> Args0Handler{
    {"--help",
     [](Arguments &s) {
         s.help = true;
         std::cout << "--help\n";
     }},
    {"-h",
     [](Arguments &s) {
         s.help = true;
         std::cout << "-h\n";
     }},

};

using Args1Func = std::function<void(Arguments &, const std::string &)>;

const std::unordered_map<std::string, Args1Func> Args1Handler{
    {"--getfile",
     [](Arguments &s, const std::string &arg) { std::cout << "--getfile\n"; }},
};

Arguments parse_args(int argc, const char *argv[]) {
    if (argc == 1) {
        std::cout << "help\n";
        return Arguments{true, ""};
    }

    Arguments args;

    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];

        if (auto handler = Args0Handler.find(arg);
            handler != Args0Handler.end()) {

            handler->second(args);
            continue;

        } else if (auto handler = Args1Handler.find(arg);
                   handler != Args1Handler.end()) {

            if (++i < argc) {
                handler->second(args, {argv[i]});
                continue;
            } else
                throw std::runtime_error{"missing param after " + arg};

        } else {

            std::cerr << "unrecognized command-line option " << arg
                      << std::endl;
        }
    }
    return Arguments{true, ""};
}