add_library(iserializable SHARED src/iserializable.cpp)

target_include_directories(iserializable PUBLIC include ../buffer/include)
target_link_libraries(iserializable PUBLIC buffer)	