#include "cli_cmds.hpp"
#include "file_system.hpp"

#include <iostream>

void ShareFile::SetOption(const Option &o) {

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

ShareFile::ShareFile() {
    Opt0Handler = handler<OptSetFunc>{
        {OPT0_HELP, [](const Option &o) { std::cout << o.flag << "\n"; }},
    };
    Opt1Handler = handler<OptSetFunc>{
        {OPT1_PATH,
         [this](const Option &o) {
             if (o.argument.has_value())
                 description = o.argument.value();
         }},
    };
}

void ShareFile::Execute(const Command &c) {
    if (c.object.has_value()) {
        std::cout << c.cmd_title << " " << c.object.value() << '\n';
        path = c.object.value();
    } else
        throw std::runtime_error{"error object for this cmd in cmd.execute()"};
}
