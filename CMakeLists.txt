cmake_minimum_required(VERSION 3.11.0)
project(flatbuffers-test VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include(CTest)
enable_testing()

include_directories(src)
add_subdirectory(src)
add_subdirectory(benchmarks)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
