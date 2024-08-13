cmake_minimum_required(VERSION 3.21)

set(SDL_STATIC ON)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

add_library(HydrogenCore STATIC src/window.cc)

target_include_directories(HydrogenCore PUBLIC include)

target_link_libraries(HydrogenCore PUBLIC SDL3::SDL3-static)