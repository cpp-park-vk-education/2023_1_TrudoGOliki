cmake_minimum_required(VERSION 3.20)
project(NET_Lib)

set(HEADERS include/connection.hpp include/server.hpp include/socket_address.hpp include/socket.hpp)
set(SOURCES src/connection.cpp src/server.cpp)

add_compile_options(-Wall -Wextra)

add_library(network ${SOURCES} ${HEADERS})
target_include_directories(network PUBLIC include)
