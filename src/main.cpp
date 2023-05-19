#include "cli.hpp"
#include "connection.hpp"

#include <iostream>

int main(int argc, char *argv[]) {

    /*CLI cliI;
    try {
        cliI.ParseArgs(argc, argv);
    } catch (const std::exception &ex) {
        throw CLI::CliParseError("in ParseArgs: " +
                                 static_cast<std::string>(ex.what()));
    } catch (...) {
        std::cerr << "Some other exception\n";
        return 1;
    }

    cliI.Execute();*/

    // Http example.com ip - 93.184.216.34
    Connection c{SocketAddress{"93.184.216.34", 80}};

    std::string get_request = "GET / HTTP/1.1\r\n"
                              "Host: example.com\r\n"
                              "User-Agent: Wget/1.21.2\r\n"
                              "Accept: */*\r\n"
                              "Accept-Encoding: identity\r\n"
                              "Connection: close\r\n"
                              "\r\n";

    c.write(get_request);
    std::string response = c.read(10000);
    std::cout << response << std::endl;

    return 0;
}