
AUX_SOURCE_DIRECTORY(. DIR_TEST_SRC)
include_directories("${PROJECT_SOURCE_DIR}/include/test")
add_executable( test_number ${DIR_TEST_SRC} )
target_link_libraries( test_number number FeNAL_sig)
