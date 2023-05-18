project(CLI_Lib)

set(SOURCES src/cli.cpp src/Option.cpp src/IExecutable.cpp)
set(HEADERS include/cli.hpp include/Option.hpp include/IExecutable.hpp)

add_library(cli ${SOURCES} ${HEADERS})
target_include_directories(cli PUBLIC include)



#add_library(cli )
#target_include_directories(cli PUBLIC include)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()