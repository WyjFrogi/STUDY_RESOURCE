cmake_minimum_required(VERSION 3.10)
project(cameracalibration)

#Add the source in project root directory
aux_source_directory(. cameracalibration)
#Add header file include directories
#include_directories(sharefile)
#Add block diectories
#add_subdirectory(sharefile)
#Target
#add_executable(cameracalibration ${DIRSRCS})
#add_library(cameracalibrationSTATIC ${DIRSRCS})
add_library(cameracalibration SHARED ${cameracalibration})
target_link_libraries(cameracalibration  ${OpenCV_LIBS})
target_link_libraries(cameracalibration  ethzasl_apriltag2)
