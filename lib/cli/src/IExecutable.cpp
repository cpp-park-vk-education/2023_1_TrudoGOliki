#include "IExecutable.hpp"
#include "Option.hpp"
#include <iostream>

Addfile::Addfile() {
    std::unordered_map<std::string_view, Opt0SetFunc> Opt0Handler{
        {OPT0_HELP,
         [](Option &o) {
             // s.help = true;
             std::cout << OPT0_HELP << "\n";
         }},
    };
    std::unordered_map<std::string_view, Opt0SetFunc> Opt1Handler{
        {OPT1_PATH, [](Option &o) { std::cout << "--path\n"; }},
    };
}

void Addfile::Execute(Command c) {
    if (c.object.has_value())
        std::cout << c.cmd_title << " " << c.object.value() << '\n';
    else
        std::cout << c.cmd_title << " " << '\n';
}
void Addfile::SetOpt0(Option o) {
    if (auto handler = Opt1Handler.find(o.flag); handler != Opt1Handler.end()) {
        handler->second(o);
    }
}