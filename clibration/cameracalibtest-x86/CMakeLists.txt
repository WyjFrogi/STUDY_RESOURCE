cmake_minimum_required(VERSION 3.10)
project(cameracalibtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)  

include_directories(${OpenCV_INCLUDE_DIRS})


find_package(Eigen3)

include_directories(${EIGEN3_INCLUDE_DIR})

find_package(ethzasl_apriltag2 REQUIRED CONFIG)

include_directories(
    /usr/include
    /usr/local/include
)

link_directories(
    ./lib
    /usr/local/lib
#      /usr/local/mdc_sdk/dp_gea/mdc_cross_compiler/sysroot/usr/lib
)
add_subdirectory(src)


