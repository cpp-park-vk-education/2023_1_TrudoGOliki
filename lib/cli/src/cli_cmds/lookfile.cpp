#include "cli_cmds.hpp"
#include "protocol.hpp"

#include <iostream>

void LookFile::SetOption(const Option &o) {

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

LookFile::LookFile() {
    Opt0Handler = handler<OptSetFunc>{
        {OPT0_HELP, [](const Option &o) { std::cout << o.flag << "\n"; }},
    };

    Opt1Handler = handler<OptSetFunc>{
        {OPT1_PATH,
         [this](const Option &o) {
             if (o.argument.has_value())
                 Path = o.argument.value();
         }},
        {OPT1_IP,
         [this](const Option &o) {
             if (o.argument.has_value())
                 Ip = o.argument.value();
         }},
    };
}

void LookFile::Execute(const Command &c) {
    if (c.object.has_value()) {
        std::cout << c.cmd_title << " " << c.object.value() << '\n';
        FID = c.object.value();
    } else
        throw std::runtime_error{"error object for this cmd in cmd.execute()"};
    Protocol p;
    p.SendFile(FID, Ip);
}
