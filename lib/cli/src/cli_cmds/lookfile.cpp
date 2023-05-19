#include "cli_cmds.hpp"

#include "connection.hpp"

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
        {OPT1_PATH, [](const Option &o) { std::cout << o.flag << "\n"; }},
        {OPT1_ADD, [](const Option &o) { std::cout << o.flag << "\n"; }},
    };
}

void LookFile::Execute(const Command &c) {
    if (c.object.has_value()) {
        std::cout << c.cmd_title << " " << c.object.value() << '\n';
        FID = c.object.value();
    } else
        throw std::runtime_error{"error object for this cmd in cmd.execute()"};

    // Http example.com ip - 93.184.216.34
    Connection con{SocketAddress{"93.184.216.34", 80}};

    std::string get_request = "GET / HTTP/1.1\r\n"
                              "Host: example.com\r\n"
                              "User-Agent: Wget/1.21.2\r\n"
                              "Accept: */*\r\n"
                              "Accept-Encoding: identity\r\n"
                              "Connection: close\r\n"
                              "\r\n";

    con.write_str(get_request);
    std::string response = con.read(10000);
    std::cout << response << std::endl;
}
