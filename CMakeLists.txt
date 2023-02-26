cmake_minimum_required(VERSION 3.11.0)
project(flatbuffers-test VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

set(CLIENT_BASE_PATH rust-fb)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CARGO_CMD cargo build)
    set(TARGET_DIR "debug")
else ()
    set(CARGO_CMD cargo build --release)
    set(TARGET_DIR "release")
endif ()

if( CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(LIBRARY_EXT "dylib")
else ()
    set(LIBRARY_EXT "so")
endif()
set(CLIENT_SO "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/librust_fb.${LIBRARY_EXT}")

add_custom_target(client ALL
    COMMENT "Compiling client module"
    COMMAND CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} ${CARGO_CMD} 
    COMMAND cp ${CLIENT_SO} ${CMAKE_CURRENT_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${CLIENT_BASE_PATH})
set_target_properties(client PROPERTIES LOCATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME client_test 
    COMMAND cargo test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    
include(CTest)
enable_testing()

include_directories(src)
add_subdirectory(src)
add_subdirectory(benchmarks)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
