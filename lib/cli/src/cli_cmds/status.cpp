#include "cli_cmds.hpp"

#include <iostream>

void Status::SetOption(const Option &o) {

    if (!o.argument.has_value()) {
        if (auto handler = Opt0Handler.find(o.flag);
            handler != Opt0Handler.end()) {
            handler->second(o);
        }
    } else {
        if (auto handler = Opt1Handler.find(o.flag);
            handler != Opt1Handler.end()) {
            handler->second(o);
        }
    }
}
//------------------START YOUR IMPLEMENTATION------------------//

Status::Status() {
    Opt0Handler = handler<OptSetFunc>{
        {OPT0_HELP, [](const Option &o) { std::cout << o.flag << "\n"; }},
    };
    Opt1Handler = handler<OptSetFunc>{
        {OPT1_PATH, [](const Option &o) { std::cout << o.flag << "\n"; }},
        {OPT1_ADD, [](const Option &o) { std::cout << o.flag << "\n"; }},
    };
}

void Status::Execute(const Command &c) {
    if (c.object.has_value()) {
        throw std::runtime_error{"error object for this cmd in cmd.execute()"};
    } else
        std::cout << c.cmd_title << " " << '\n';
}
