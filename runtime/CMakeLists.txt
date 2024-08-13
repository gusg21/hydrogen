cmake_minimum_required(VERSION 3.21)

add_executable(HydrogenRuntime src/main.cc)

target_link_libraries(HydrogenRuntime PUBLIC HydrogenCore)
