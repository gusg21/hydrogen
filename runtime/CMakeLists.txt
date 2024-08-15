cmake_minimum_required(VERSION 3.21)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}>)
add_executable(HydrogenRuntime src/main.cc)

target_link_libraries(HydrogenRuntime PUBLIC HydrogenCore)

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})