add_library(cli src/cli.cpp src/Option.cpp src/IExecutable.cpp)
target_include_directories(cli PUBLIC include)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()