
add_library(file src/file.cpp)

target_include_directories(file PUBLIC include include ../iserializable/include)
target_link_libraries(file iserializable)
