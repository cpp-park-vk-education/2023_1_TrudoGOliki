#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <utility>
#include <string_view>

#include "client.hpp"


int main(int argc, char* argv[]) {
    NetworkInfo my_info((in_addr_t) INADDR_LOOPBACK, 8080, true);
    return 0;
}
