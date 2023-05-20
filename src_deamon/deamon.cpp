#include "server.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    Server s{SocketAddress{argv[1], 8080}};
    s.Run();
}
