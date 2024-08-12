cmake_minimum_required(VERSION 3.21)

add_library(HydrogenCore STATIC src/window.cc)

target_link_libraries(HydrogenCore PUBLIC SDL2::SDL2-static)