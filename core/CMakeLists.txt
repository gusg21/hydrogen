cmake_minimum_required(VERSION 3.21)



# Core sources
add_library(HydrogenCore STATIC 

    src/actorspec.cc                    include/core/actorspec.h
    src/asset.cc                        include/core/asset.h
    src/assets.cc                       include/core/assets.h
    src/engine.cc                       include/core/engine.h
                                        include/core/engineevents.h
    src/event.cc                        include/core/event.h
    src/eventqueue.cc                   include/core/eventqueue.h
    src/mesh.cc                         include/core/mesh.h
    src/project.cc                      include/core/project.h
    src/scene.cc                        include/core/scene.h
    src/system.cc                       include/core/system.h
    src/scenespec.cc                    include/core/scenespec.h
    src/transform.cc                    include/core/transform.h
    src/window.cc                       include/core/window.h
                                    
    src/math/mat4x4.cc                  include/core/math/mat4x4.h
                                        include/core/math/math.h
    src/math/quaternion.cc              include/core/math/quaternion.h
    src/math/vector2.cc                 include/core/math/vector2.h
    src/math/vector3.cc                 include/core/math/vector3.h

    src/systems/sys_gravity.cc          include/core/systems/sys_gravity.h
    src/systems/renderer/renderer.cc    include/core/systems/renderer/renderer.h
    src/systems/renderer/shader.cc      include/core/systems/renderer/shader.h
                                        include/core/systems/renderer/vertex.h
    )
if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:library")
endif()
target_compile_definitions(HydrogenCore PRIVATE "HCORE_DEBUG=$<IF:$<CONFIG:Debug>,1,0>")

# Core include dir
target_include_directories(HydrogenCore PUBLIC include)

# SDL
set(SDL_STATIC ON)
set(SDL_DISABLE_UNINSTALL ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PUBLIC SDL2::SDL2-static)
if(TARGET SDL2::SDL2main)
    target_link_libraries(HydrogenCore PRIVATE SDL2::SDL2main)
endif()

set_target_properties(SDL2-static PROPERTIES FOLDER "SDL")

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(HydrogenCore PUBLIC ${OPENGL_LIBRARIES})

# GLM
set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(external/glm)
target_link_libraries(HydrogenCore PUBLIC glm::glm)

# GLAD
add_subdirectory(external/glad)
target_link_libraries(HydrogenCore PUBLIC glad)

# ImGui integrations
add_subdirectory(external/imgui.cmake EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PUBLIC imgui.cmake)
target_include_directories(HydrogenCore PRIVATE external/sdl-imgui)
target_include_directories(HydrogenCore PRIVATE external/opengl-imgui)

target_sources(HydrogenCore PRIVATE 
    external/sdl-imgui/imgui_impl_sdl2.cpp
    external/opengl-imgui/imgui_impl_opengl3.cpp
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

# generated shader code
function(HydrogenCore_CopyShaders folder)
    add_custom_target(HydrogenCore_CopyShaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/shaders hcore_assets
    )
    add_dependencies(HydrogenCore HydrogenCore_CopyShaders)
endfunction()

function(HydrogenCore_SetupMain target)
    if(TARGET SDL2::SDL2main)
        target_link_libraries(${target} PRIVATE SDL2::SDL2main)
    endif()

    target_sources(${target} PRIVATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/icon/app.o)
endfunction()