
cmake_minimum_required(VERSION 3.8)

PROJECT(NUMBER CXX)

include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

ADD_SUBDIRECTORY(src)

enable_testing()
add_test( NAME test-all
          COMMAND test_number )
