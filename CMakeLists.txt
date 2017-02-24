
cmake_minimum_required(VERSION 2.8)

PROJECT(NUMBER CXX)
include_directories("${PROJECT_SOURCE_DIR}/include")
ADD_SUBDIRECTORY(src)

enable_testing()
add_test( NAME test-all
          COMMAND test_number )
