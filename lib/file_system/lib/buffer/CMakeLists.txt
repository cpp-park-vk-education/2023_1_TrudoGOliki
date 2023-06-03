add_library(buffer SHARED src/buffer.cpp)

target_include_directories(buffer PUBLIC include ../errors/include)
target_link_libraries(buffer errors)