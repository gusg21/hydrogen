cmake_minimum_required(VERSION 3.21)



# Core sources
add_library(HydrogenCore STATIC 

                                    include/core/actor.h
    src/actorspec.cc                include/core/actorspec.h
    src/asset.cc                    include/core/asset.h
    src/assets.cc                   include/core/assets.h
    src/engine.cc                   include/core/engine.h
                                    include/core/engineevents.h
    src/event.cc                    include/core/event.h
    src/eventqueue.cc               include/core/eventqueue.h
    src/model.cc                    include/core/model.h
    src/project.cc                  include/core/project.h
    src/scene.cc                    include/core/scene.h
    src/system.cc                   include/core/system.h
    src/scenespec.cc                include/core/scenespec.h
    src/transform.cc                include/core/transform.h
    src/window.cc                   include/core/window.h
                                    
    src/math/mat4x4.cc              include/core/math/mat4x4.h
    src/math/vector2.cc             include/core/math/vector2.h
    src/math/vector3.cc             include/core/math/vector3.h

    src/systems/sys_gravity.cc      include/core/systems/sys_gravity.h
    src/systems/sys_rendering.cc    include/core/systems/sys_rendering.h

    )

# Core include dir
target_include_directories(HydrogenCore PUBLIC include)

# SDL
set(SDL_STATIC OUTPUT_NAME_<CONFIG>)
set(SDL_DISABLE_UNINSTALL ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF )
set(SDL_EXAMPLES OFF)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PUBLIC SDL3::SDL3-static)

set_target_properties(SDL3-shared PROPERTIES FOLDER "SDL")
set_target_properties(SDL3-static PROPERTIES FOLDER "SDL")

# BGFX
set(BGFX_CUSTOM_TARGETS OFF)
set(BGFX_BUILD_TOOLS OFF)
set(BGFX_INSTALL OFF)
add_subdirectory(external/bgfx.cmake EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PUBLIC bgfx)
target_link_libraries(HydrogenCore PRIVATE bx)
target_link_libraries(HydrogenCore PRIVATE bimg)

# ImGui integrations
add_subdirectory(external/imgui.cmake EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PUBLIC imgui.cmake)
target_include_directories(HydrogenCore PRIVATE external/sdl-imgui)
target_include_directories(HydrogenCore PRIVATE external/bgfx-imgui)
target_sources(HydrogenCore PRIVATE 
    external/sdl-imgui/imgui_impl_sdl3.cpp external/bgfx-imgui/imgui_impl_bgfx.cpp
    )

set_target_properties(imgui.cmake PROPERTIES FOLDER "imgui")

# tinygltf integration
set(TINYGLTF_INSTALL OFF)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)
add_subdirectory(external/tinygltf EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PRIVATE tinygltf)

set_target_properties(tinygltf PROPERTIES FOLDER "tinygltf")

# yaml-cpp
set(YAML_BUILD_SHARED_LIBS OFF)
set(YAML_CPP_FORMAT_SOURCE OFF)
set(YAML_CPP_INSTALL OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(external/yaml-cpp EXCLUDE_FROM_ALL)
set_target_properties(yaml-cpp PROPERTIES FOLDER "yaml-cpp")
set_target_properties(uninstall PROPERTIES FOLDER "yaml-cpp/misc")

target_link_libraries(HydrogenCore PUBLIC yaml-cpp::yaml-cpp)