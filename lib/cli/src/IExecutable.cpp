#include "IExecutable.hpp"
#include "Option.hpp"

#include <iostream>
#include <memory>

LookFile::LookFile() {
    Opt0Handler = handler<OptSetFunc>{
        {OPT0_HELP, [](const Option &o) { std::cout << OPT0_HELP << "\n"; }},
    };
    Opt1Handler = handler<OptSetFunc>{
        {OPT1_PATH, [](const Option &o) { std::cout << "--path\n"; }},
        {OPT1_ADD, [](const Option &o) { std::cout << "--add\n"; }},
    };
}

void LookFile::Execute(const Command &c) {
    if (c.object.has_value()) {
        std::cout << c.cmd_title << " " << c.object.value() << '\n';
        FID = c.object.value();
    } else
        std::cout << c.cmd_title << " " << '\n';
}
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