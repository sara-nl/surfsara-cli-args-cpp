cmake_minimum_required (VERSION 3.5)
project (CliArg)
set (CliArg_VERSION_MAJOR 1)
set (CliArg_VERSION_MINOR 0)
set (CMAKE_CXX_STANDARD 11)

include_directories (${PROJECT_SOURCE_DIR}/Catch2/single_include)
include_directories (${PROJECT_SOURCE_DIR}/include)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE test)

add_executable(runtest
      tests/runtests.cpp
      tests/test_argument.cpp
      tests/test_flag.cpp
      tests/test_multiple_flag.cpp
      tests/test_value.cpp
      tests/test_multiple_value.cpp
      tests/test_parser.cpp
      )
add_executable(example example.cpp)
target_link_libraries(runtest Catch)
