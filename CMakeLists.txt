cmake_minimum_required(VERSION 3.20)
project(Vkyrse VERSION 0.1.0)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_subdirectory(lib/cli)
add_subdirectory(lib/network)
    
add_executable(Vkyrse src/main.cpp)
target_link_libraries(Vkyrse cli network)
