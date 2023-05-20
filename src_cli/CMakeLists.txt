cmake_minimum_required(VERSION 3.20)
project(Vkyrse VERSION 0.1.0)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_subdirectory(./../lib/cli cli)
add_subdirectory(./../lib/network network)
add_subdirectory(./../lib/file_system file_system)
add_subdirectory(./../lib/protocol protocol)
    
add_executable(Vkyrse cli.cpp)
target_link_libraries(Vkyrse cli network file_system protocol)
