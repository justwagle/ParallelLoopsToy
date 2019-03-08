cmake_minimum_required(VERSION 3.6)
project(Parceltoy CXX)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -w")

# Find and include HPX settings
if(NOT HPX_DIR)
    message(FATAL "HPX_DIR not set, unable to find HPX!")
endif()

message(STATUS "HPX_DIR=${HPX_DIR}")

# Instruct cmake to find the HPX settings
find_package(HPX NO_CMAKE_PACKAGE_REGISTRY REQUIRED)
if(HPX_FOUND)
    include_directories(${HPX_INCLUDE_DIR})
    link_directories(${HPX_LIBRARY_DIR})
endif()

add_executable(parceltoy pingpong_performance.cpp)
hpx_setup_target(parceltoy COMPONENT_DEPENDENCIES iostreams)

