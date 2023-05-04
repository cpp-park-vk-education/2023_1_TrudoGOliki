#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <utility>
#include <string_view>
#include <vector>

struct NetworkInfo{
    in_addr_t ip_;
    in_port_t port_;
    bool status_;
    NetworkInfo(in_addr_t &ip, in_port_t &port, bool &status):ip_(ip), port_(port), status_(status){}
};

struct NetworkUser{
    NetworkInfo network_info_;
    NetworkUser(NetworkInfo &network_info):network_info_(network_info){}
};

class Client{
    private:
    NetworkInfo network_info_;
    std::vector<NetworkUser> companions_;
    public:
    Client(NetworkInfo &network_info, std::vector<NetworkUser> &companions):network_info_(network_info), companions_(companions){}
    NetworkUser findSuitableCompanion(std::vector<NetworkUser> &companions);
    void connectWithCompanion(NetworkUser &companion);
};

void Client::connectWithCompanion(NetworkUser &companion){
    int socket = ::socket(AF_INET, SOCK_STREAM, 0);

    ::sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = companion.network_info_.port_;
    addr.sin_addr.s_addr = companion.network_info_.ip_;

    int result = ::connect(socket, reinterpret_cast<::sockaddr*>(&addr), sizeof(addr));
    if (result != 0){
        throw std::runtime_error("Cannot connect");
    }
    const char *msg = "Hello\n";
    std::cerr << ::write(socket, msg, std::strlen(msg)) << std::endl;

    char BUFFER[1024];
    std::cerr << ::read(socket, BUFFER, 1024) << std::endl;
    std::cout << BUFFER << std::endl;
    ::close(socket);
}