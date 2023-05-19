#include "cli.hpp"
#include "Option.hpp"
#include <functional>
#include <iostream>
#include <memory>
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

/*using Cmd0Func = std::function<void(Option &)>;

const std::unordered_map<std::string_view, Cmd0Func> Cmd0Handler{
    {"ghj",
     [](Option &s) {
         // s.help = true;
         std::cout << "homedata\n";
     }},
    {"gh",
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

};*/

CLI::CLI() {
    LookFile LookFile_;
    // IExecutablePtr ex = std::make_shared<LookFile>(LookFile_);

    CmdHandler = handler<IExecutablePtr>{
        {"lookfile", std::make_shared<LookFile>(LookFile_)},
        // ex,
    };

    Opt0Set = checker{
        OPT0_HELP,
        OPT0_UPDATE,
        OPT0_STATUS,
    };
    Opt1Set = checker{
        OPT1_ADD, OPT1_PATH, OPT1_REMOVE, OPT1_MKDIR, OPT1_GETFILE, OPT1_TIMETO,
    };
    Cmd0Set = checker{"ghj", "jdjd"};
    Cmd1Set = checker{"send", "lookfile"};
}

void CLI::Execute() {
    auto CmdExecutable = std::move(CmdHandler[cmd.cmd_title]);

    for (auto &o : options)
        CmdExecutable->SetOption(o);

    CmdExecutable->Execute(cmd);
}

void CLI::ParseArgs(int argc, char *argv[]) {

    if (argc == 1) {
        std::cout << "help\n";
        options.push_back(Option{"-h", {}});
        return;
    }

    bool need_object = false;

    if (auto handler = Cmd0Set.find(argv[1]); handler != Cmd0Set.end()) {
    } else if (auto handler = Cmd1Set.find(argv[1]); handler != Cmd1Set.end()) {
        need_object = true;
    } else {
        throw std::runtime_error{"unknow command"};
    }

    int i = 2;
    for (; i < (argc - need_object); i++) {
        std::string_view arg = argv[i];

        if (auto handler = Opt0Set.find(arg); handler != Opt0Set.end()) {

            options.push_back(Option{arg, {}});
            continue;

        } else if (auto handler = Opt1Set.find(arg); handler != Opt1Set.end()) {

            if (++i < (argc - need_object)) {

                options.push_back(Option{arg, {argv[i]}});
                continue;
            } else
                throw std::runtime_error{"missing param after " +
                                         std::string{arg}};

        } else {
            throw std::runtime_error{"unrecognized command-line option " +
                                     std::string{arg}};
        }
    }

    if (need_object) {
        if (!(i < argc))
            throw std::runtime_error{"missing [OBJECT] for command " +
                                     std::string{argv[1]}};
        cmd = {argv[1], {argv[i]}};
    } else
        cmd = {argv[1], {}};

    // cmd = need_object ? ({argv[1], {argv[i]}}) : ({argv[1], {}});

    // PRINT PARSE REZULT
    std::cout << cmd.cmd_title << "\n";
    for (auto o : options) {
        if (o.argument.has_value())
            std::cout << o.flag << " | " << o.argument.value() << std::endl;
        else
            std::cout << o.flag << std::endl;
    }

    if (cmd.object.has_value())
        std::cout << cmd.object.value() << "\n";
}