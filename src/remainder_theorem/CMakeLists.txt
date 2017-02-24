
aux_source_directory(. DIR_REMAINDER_THEOREM_SRC)
add_executable(remainder_theorem ${DIR_REMAINDER_THEOREM_SRC} )
include_directories(${PROJECT_SOURCE_DIR}/inlcude/remainder_theorem)
target_link_libraries( remainder_theorem number )
