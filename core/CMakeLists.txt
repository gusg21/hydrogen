cmake_minimum_required(VERSION 3.21)

# SDL
set(SDL_STATIC ON)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

# BGFX
add_subdirectory(external/bgfx.cmake EXCLUDE_FROM_ALL)

# Core sources
add_library(HydrogenCore STATIC src/window.cc src/engine.cc src/project.cc src/eventqueue.cc src/event.cc)

# Core include dir
target_include_directories(HydrogenCore PUBLIC include)

# Link
target_link_libraries(HydrogenCore PUBLIC SDL3::SDL3-static)
target_link_libraries(HydrogenCore PUBLIC bgfx)
target_link_libraries(HydrogenCore PUBLIC bx)
target_link_libraries(HydrogenCore PUBLIC bimg)