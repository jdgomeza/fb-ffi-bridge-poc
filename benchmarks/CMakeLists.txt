include(FetchContent)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz
)

FetchContent_Declare(
  benchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.7.1.tar.gz
)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)

FetchContent_MakeAvailable(googletest benchmark json)

set(BINARY ${CMAKE_PROJECT_NAME}_bench)

add_executable(${BINARY} simple_message.cpp batch_messages.cpp simple_json.cpp)
add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib 
  benchmark::benchmark nlohmann_json::nlohmann_json)
