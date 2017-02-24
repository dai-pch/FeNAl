
AUX_SOURCE_DIRECTORY(. DIR_NUMBER_SRC)
include_directories("${PROJECT_SOURCE_DIR}/include/number")
ADD_LIBRARY( number ${DIR_NUMBER_SRC} )
