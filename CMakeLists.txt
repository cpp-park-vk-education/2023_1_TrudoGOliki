cmake_minimum_required(VERSION 3.0.0)
project(vkyrse VERSION 0.1.0)

add_subdirectory(lib/file_system)

add_executable(vkyrse src/main.cpp)
target_link_libraries(vkyrse file_system)
