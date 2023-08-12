cmake_minimum_required(VERSION 3.10)
project(main)

#Add the source in project root directory
aux_source_directory(. main)
#Add block diectories
add_subdirectory(cameracalibration)
#Add header file include directories
include_directories(cameracalibration)
#Target
add_executable(${PROJECT_NAME} ${main})
target_link_libraries(${PROJECT_NAME}  cameracalibration)

