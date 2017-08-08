
AUX_SOURCE_DIRECTORY(. DIR_SIGPRO_DIR)
include_directories("${PROJECT_SOURCE_DIR}/include/signal_processing")
ADD_LIBRARY( FeNAL_sig ${DIR_SIGPRO_DIR} )

