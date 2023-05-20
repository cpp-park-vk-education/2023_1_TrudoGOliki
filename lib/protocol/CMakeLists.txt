cmake_minimum_required(VERSION 3.20)
project(PROTOCOL_Lib)

set(HEADERS include/protocol.hpp)
set(SOURCES src/protocol.cpp)


add_compile_options(-Wall -Wextra)

add_library(protocol ${SOURCES} ${HEADERS})
target_link_libraries(protocol network file_system)
target_include_directories(protocol PUBLIC include)
