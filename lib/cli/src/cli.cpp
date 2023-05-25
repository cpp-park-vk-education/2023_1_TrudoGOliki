#include "cli.hpp"
#include "Option.hpp"
#include "cli_cmds.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <string_view>
#include <unordered_set>

CLI::CLI() {
    Help Help_;
    Status Status_;
    LookFile LookFile_;
    ShareFile ShareFile_;
    HideFile HideFile_;
    // IExecutablePtr ex = std::make_shared<LookFile>(LookFile_);

    CmdHandler = handler<IExecutablePtr>{
        {CMD0_HELP, std::make_shared<Help>(Help_)},
        {CMD0_HELP, std::make_shared<Status>(Status_)},
        {CMD1_LOOKFILE, std::make_shared<LookFile>(LookFile_)},
        {CMD1_SHAREFILE, std::make_shared<ShareFile>(ShareFile_)},
        {CMD0_HELP, std::make_shared<HideFile>(HideFile_)},
        // ex,
    };

    Opt0Set = checker{
        OPT0_HELP,
        OPT0_UPDATE,
        OPT0_STATUS,
    };
    Opt1Set = checker{
        OPT1_ADD,     OPT1_PATH,   OPT1_REMOVE, OPT1_MKDIR,
        OPT1_GETFILE, OPT1_TIMETO, OPT1_IP,
    };
    Cmd0Set = checker{CMD0_HELP, CMD0_fff, CMD0_ttt};
    Cmd1Set = checker{CMD1_LOOKFILE, CMD1_SHAREFILE};
}

void CLI::Execute() {
    if (auto handler = CmdHandler.find(cmd.cmd_title);
        handler != CmdHandler.end()) {

        auto CmdExecutable = move(handler->second);
        for (auto &o : options)
            CmdExecutable->SetOption(o);

        CmdExecutable->Execute(cmd);
    } else {
        throw std::runtime_error{"can not find cmd in cli.cmd"};
    }
}

void CLI::ParseArgs(int argc, char *argv[]) {

    if (argc == 1) {
        cmd = {CMD0_HELP, {}};
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