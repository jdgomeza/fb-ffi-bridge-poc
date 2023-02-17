set(BINARY ${CMAKE_PROJECT_NAME})
# file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

include(FetchContent)
FetchContent_Declare(
  FlatBuffers
  GIT_REPOSITORY https://github.com/google/flatbuffers.git
  GIT_TAG v23.1.21
)
FetchContent_MakeAvailable(FlatBuffers)

add_library(${BINARY}_lib STATIC lib.cpp lib.h player_events_generated.h)

target_compile_features(${BINARY}_lib PUBLIC cxx_std_11)
target_link_libraries(${BINARY}_lib PUBLIC FlatBuffers)
target_include_directories(${BINARY}_lib
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE   ${CMAKE_CURRENT_BINARY_DIR}
          )
