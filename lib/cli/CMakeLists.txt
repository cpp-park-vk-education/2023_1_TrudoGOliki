add_library(cli src/cli.cpp)
target_include_directories(cli PUBLIC include)

set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/IExecutable.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/Option.hpp"
            )

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()