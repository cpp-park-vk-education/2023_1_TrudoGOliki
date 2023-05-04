cmake_minimum_required(VERSION 3.0.0)
project(dz1 VERSION 0.1.0)

add_subdirectory(lib/file_system)

add_executable(dz1 src/main.cpp)
target_link_libraries(dz1 file_system)
