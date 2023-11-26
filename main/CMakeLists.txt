cmake_minimum_required(VERSION 3.18.4)

project(roboleg_controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -o")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lbcm2835 ")

find_package(Eigen3 REQUIRED HINTS ${Eigen3_HINT})
find_library(BCM2835_LIB bcm2835)
find_library(PT_LIBRARIES pthread)
find_package(Eigen3 REQUIRED HINTS ${Eigen3_HINT})

include_directories(include)
#include_directories(/usr/local/include)
#include_directories(/home/baris/roboleg/include)
include_directories(/home/baris/include)
include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${WIRINGPI_INCLUDE_DIRS})
include_directories(${BCM2835_INCLUDE_DIRS})
include(FindPackageHandleStandardArgs)


add_definitions(-lpthread)
add_definitions(-lbcm2835)

add_executable(main main.cpp)

target_link_libraries(main pthread)
target_link_libraries(main Eigen3::Eigen)
target_link_libraries(main bcm2835)

