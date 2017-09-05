IF (UNIX)
    add_compile_options( -pg )
ENDIF ()

AUX_SOURCE_DIRECTORY(. DIR_FFT_PROFILE_SRC)
add_executable( FFT_profile ${DIR_FFT_PROFILE_SRC} )
target_link_libraries( FFT_profile number)

IF (UNIX)
    target_link_libraries( FFT_profile -pg )
ENDIF ()

SET( RUN_FFT_PROFILE_SCRIPT "${CMAKE_BINARY_DIR}/prof_fft.sh" )
FILE(WRITE ${RUN_FFT_PROFILE_SCRIPT} 
    "make FFT_profile\n./src/FFT_profile/FFT_profile\n"
)
FILE(APPEND ${RUN_FFT_PROFILE_SCRIPT} 
    "gprof -b ./src/FFT_profile/FFT_profile gmon.out >fft.report\n\n"
)
EXEC_PROGRAM(chmod ARGS "754 prof_fft.sh")
