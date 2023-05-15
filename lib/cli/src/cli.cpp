#include "cli.hpp"

#include <functional>
#include <iostream>

// void CLI::Execute() { parse_args(); }

using Args0Func = std::function<void(Option &)>;

const std::unordered_map<std::string_view, Args0Func> Args0Handler{
    {"--help",
     [](Option &s) {
         // s.help = true;
         std::cout << "--help\n";
     }},
    {"-h",
     [](Option &s) {
         // s.help = true;
         std::cout << "-h\n";
     }},
    {"--update",
     [](Option &s) {
         // s.help = true;
         std::cout << "--update\n";
     }},
    {"-status",
     [](Option &s) {
         // s.help = true;
         std::cout << "--status\n";
     }},

};

using Args1Func = std::function<void(Option &, const std::string &)>;

const std::unordered_map<std::string_view, Args1Func> Args1Handler{
    {"--add",
     [](Option &s, const std::string &arg) { std::cout << "--add\n"; }},
    {"--path",
     [](Option &s, const std::string &arg) { std::cout << "--path\n"; }},
    {"--remove",
     [](Option &s, const std::string &arg) { std::cout << "--remove\n"; }},
    {"--mkdir",
     [](Option &s, const std::string &arg) { std::cout << "--mkdir\n"; }},
    {"--getfile",
     [](Option &s, const std::string &arg) { std::cout << "--getfile\n"; }},
    {"--timeto",
     [](Option &s, const std::string &arg) { std::cout << "--timeto\n"; }},
};

using CommandFunc = std::function<void(Option &)>;

const std::unordered_map<std::string_view, CommandFunc> CommandHandler{
    {"homedata",
     [](Option &s) {
         // s.help = true;
         std::cout << "homedata\n";
     }},
    {"lookup",
     [](Option &s) {
         // s.help = true;
         std::cout << "lookup\n";
     }},

};

void CLI::ParseArgs(int argc, char *argv[]) {

    std::vector<std::string_view> args(argc);

    for (int i = 0; i < argc; ++i) {
        args[i] = argv[i];
    }

    if (argc == 1) {
        std::cout << "help\n";
        options.push_back(Option{"-h", {}});
        return;
    }

    if (auto handler = CommandHandler.find(argv[1]);
        handler != CommandHandler.end()) {

        command = argv[1];

    } else {
        throw std::runtime_error{"unknow command"};
    }

    for (int i = 2; i < argc; i++) {
        std::string_view arg = argv[i];

        if (auto handler = Args0Handler.find(arg);
            handler != Args0Handler.end()) {

            options.push_back(Option{arg, {}});
            continue;

        } else if (auto handler = Args1Handler.find(arg);
                   handler != Args1Handler.end()) {

            if (++i < argc) {
                // handler->second(args, {argv[i]});
                options.push_back(Option{arg, {argv[i]}});
                continue;
            } else
                throw std::runtime_error{"missing param after " +
                                         std::string{arg}};

        } else {

            std::cerr << "unrecognized command-line option " << arg
                      << std::endl;

            throw std::runtime_error{"undefined " + std::string{arg}};
        }
    }

    std::cout << command << " argS:\n";
    for (auto o : options) {
        if (o.argument.has_value())
            std::cout << o.flag << " | " << o.argument.value() << std::endl;
        else
            std::cout << o.flag << std::endl;
    }
}