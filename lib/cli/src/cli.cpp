#include "cli.hpp"
#include "Option.hpp"
#include <functional>
#include <iostream>
#include <string_view>
#include <unordered_set>

/*using Opt0SetFunc = std::function<void(Option &, IExecutablePtr)>;

const std::unordered_map<std::string_view, Opt0SetFunc> Opt0Handler{
    {OPT0_HELP,
     [](Option &o, IExecutablePtr e) {
         // s.help = true;
         std::cout << OPT0_HELP << "\n";
     }},
    {"-h",
     [](Option &o, IExecutablePtr e) {
         // s.help = true;
         std::cout << "-h\n";
     }},
    {OPT0_UPDATE,
     [](Option &o, IExecutablePtr e) {
         // s.help = true;
         std::cout << OPT0_UPDATE << "\n";
     }},
    {OPT0_STATUS,
     [](Option &o, IExecutablePtr e) {
         // s.help = true;
         std::cout << OPT0_STATUS << "\n";
     }},

};

using Opt1SetFunc = std::function<void(Option &, IExecutablePtr)>;

const std::unordered_map<std::string_view, Opt1SetFunc> Opt1Handler{
    {OPT1_ADD,
     [](Option &o, IExecutablePtr e) {
         e->Set() = 9;
         std::cout << "--add\n";
     }},
    {OPT1_PATH, [](Option &o, IExecutablePtr e) { std::cout << "--path\n"; }},
    {OPT1_REMOVE,
     [](Option &o, IExecutablePtr e) { std::cout << "--remove\n"; }},
    {OPT1_MKDIR, [](Option &o, IExecutablePtr e) { std::cout << "--mkdir\n"; }},
    {OPT1_GETFILE,
     [](Option &o, IExecutablePtr e) { std::cout << "--getfile\n"; }},
    {OPT1_TIMETO,
     [](Option &o, IExecutablePtr e) { std::cout << "--timeto\n"; }},
};
*/

const std::unordered_set<std::string_view> Opt0Handler{
    OPT0_HELP,
    OPT0_UPDATE,
    OPT0_STATUS,
};

const std::unordered_set<std::string_view> Opt1Handler{
    OPT1_ADD, OPT1_PATH, OPT1_REMOVE, OPT1_MKDIR, OPT1_GETFILE, OPT1_TIMETO,
};

using Cmd0Func = std::function<void(Option &)>;

const std::unordered_map<std::string_view, Cmd0Func> Cmd0Handler{
    {"addfile",
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

using Cmd1Func = std::function<void(Option &)>;

const std::unordered_map<std::string_view, Cmd1Func> Cmd1Handler{
    {"addfile",
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

void CLI::Execute() {
    auto CmdHandler = std::move(handler[cmd.cmd_title]);

    for (auto &o : options) {
        if (o.argument.has_value()) {
            // auto h = Opt1Handler.at(o.flag);
            CmdHandler->SetOpt1(o);
        } else {
            // auto h = Opt0Handler.at(o.flag);
            // h(o, CmdHandler);
            CmdHandler->SetOpt0(o);
        }
    }
    CmdHandler->Execute(cmd);
}

void CLI::ParseArgs(int argc, char *argv[]) {

    if (argc == 1) {
        std::cout << "help\n";
        options.push_back(Option{"-h", {}});
        return;
    }

    bool need_object = false;

    if (auto handler = Cmd0Handler.find(argv[1]);
        handler != Cmd0Handler.end()) {

        cmd = {argv[1], {}};
        need_object = true;

    } else {
        throw std::runtime_error{"unknow command"};
    }

    if (auto handler = Cmd1Handler.find(argv[1]);
        handler != Cmd1Handler.end()) {
        need_object = true;

    } else {
        throw std::runtime_error{"unknow command"};
    }

    int i = 2;
    for (; i < (argc - need_object); i++) {
        std::string_view arg = argv[i];

        if (auto handler = Opt0Handler.find(arg);
            handler != Opt0Handler.end()) {

            options.push_back(Option{arg, {}});
            continue;

        } else if (auto handler = Opt1Handler.find(arg);
                   handler != Opt1Handler.end()) {

            if (i < (argc - need_object)) {
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
    if (need_object)
        cmd = {argv[1], {argv[++i]}};
    else
        cmd = {argv[1], {}};

    // cmd = need_object ? ({argv[1], {argv[i]}}) : ({argv[1], {}});

    std::cout << cmd.cmd_title << " argS:\n";
    for (auto o : options) {
        if (o.argument.has_value())
            std::cout << o.flag << " | " << o.argument.value() << std::endl;
        else
            std::cout << o.flag << std::endl;
    }
}