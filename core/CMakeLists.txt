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
        src/project.cc                      include/core/project.h
        src/scene.cc                        include/core/scene.h
        src/system.cc                       include/core/system.h
        src/systems.cc                      include/core/systems.h
        src/scenespec.cc                    include/core/scenespec.h
        src/transform.cc                    include/core/transform.h
        src/window.cc                       include/core/window.h

        src/math/mat4x4.cc                  include/core/math/mat4x4.h
                                            include/core/math/math.h
        src/math/quaternion.cc              include/core/math/quaternion.h
        src/math/vector2.cc                 include/core/math/vector2.h
        src/math/vector3.cc                 include/core/math/vector3.h

        src/systems/gravity.cc              include/core/systems/gravity.h
        src/systems/render/mesh.cc          include/core/systems/render/mesh.h
        src/systems/render/renderer.cc      include/core/systems/render/renderer.h
        src/systems/render/shader.cc        include/core/systems/render/shader.h
                                            include/core/systems/render/vertex.h
        src/systems/script/scriptasset.cc   include/core/systems/script/scriptasset.h
        src/systems/script/scriptcomp.cc    include/core/systems/script/scriptcomp.h
        src/systems/script/scripting.cc     include/core/systems/script/scripting.h
    )
if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:library")
endif()
target_compile_definitions(HydrogenCore PRIVATE "HCORE_DEBUG=$<IF:$<CONFIG:Debug>,1,0>")

# Core include dir
target_include_directories(HydrogenCore PUBLIC include)

# AngelScript
add_subdirectory(external/angelscriptsdk/angelscript/projects/cmake)
add_library(HydrogenCore_AngelScriptExtensions STATIC 
    external/angelscriptsdk/add_on/scriptstdstring/scriptstdstring.cpp
)
target_include_directories(HydrogenCore_AngelScriptExtensions PUBLIC 
    external/angelscriptsdk/add_on/scriptstdstring/
)
target_link_libraries(HydrogenCore_AngelScriptExtensions PUBLIC angelscript)
target_link_libraries(HydrogenCore PUBLIC HydrogenCore_AngelScriptExtensions)
set_target_properties(angelscript PROPERTIES FOLDER "Vendored/AngelScript")
set_target_properties(HydrogenCore_AngelScriptExtensions PROPERTIES FOLDER "Vendored/AngelScript")

# SDL
set(SDL_STATIC ON)
set(SDL_SHARED ON) # not required but sometimes SDL will not build without it :(
set(SDL_DISABLE_UNINSTALL ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PUBLIC SDL2::SDL2-static)
if(TARGET SDL2::SDL2main)
    target_link_libraries(HydrogenCore PRIVATE SDL2::SDL2main)
endif()

set_target_properties(SDL2-static PROPERTIES FOLDER "Vendored/SDL")
set_target_properties(SDL2_test PROPERTIES FOLDER "Vendored/SDL")
set_target_properties(SDL2main PROPERTIES FOLDER "Vendored/SDL")
set_target_properties(sdl_headers_copy PROPERTIES FOLDER "Vendored/SDL")
if(TARGET SDL2)
    set_target_properties(SDL2 PROPERTIES FOLDER "Vendored/SDL") # see above comment
endif()

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(HydrogenCore PUBLIC ${OPENGL_LIBRARIES})

# GLM
set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(external/glm)
target_link_libraries(HydrogenCore PUBLIC glm::glm)
set_target_properties(glm PROPERTIES FOLDER "Vendored/GLM")

# GLAD
add_subdirectory(external/glad)
target_link_libraries(HydrogenCore PUBLIC glad)
set_target_properties(glad PROPERTIES FOLDER "Vendored/glad")

# ImGui integrations
add_subdirectory(external/imgui.cmake EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PUBLIC imgui.cmake)
target_include_directories(HydrogenCore PRIVATE external/sdl-imgui)
target_include_directories(HydrogenCore PRIVATE external/opengl-imgui)

target_sources(HydrogenCore PRIVATE 
    external/sdl-imgui/imgui_impl_sdl2.cpp
    external/opengl-imgui/imgui_impl_opengl3.cpp
    )

set_target_properties(imgui.cmake PROPERTIES FOLDER "Vendored/imgui")

# tinygltf integration
set(TINYGLTF_INSTALL OFF)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)
add_subdirectory(external/tinygltf EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PRIVATE tinygltf)

set_target_properties(tinygltf PROPERTIES FOLDER "Vendored/tinygltf")

# yaml-cpp
set(YAML_BUILD_SHARED_LIBS OFF)
set(YAML_CPP_FORMAT_SOURCE OFF)
set(YAML_CPP_INSTALL OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(external/yaml-cpp EXCLUDE_FROM_ALL)
set_target_properties(yaml-cpp PROPERTIES FOLDER "Vendored/yaml-cpp")
set_target_properties(uninstall PROPERTIES FOLDER "Vendored/yaml-cpp/misc")

target_link_libraries(HydrogenCore PUBLIC yaml-cpp::yaml-cpp)

# generated shader code
function(HydrogenCore_CopyShaders folder)
    add_custom_target(HydrogenCore_CopyShaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/shaders hcore_assets
    )
    add_dependencies(HydrogenCore HydrogenCore_CopyShaders)
    set_target_properties(HydrogenCore_CopyShaders PROPERTIES FOLDER "HyCoreSupport")
endfunction()

# setup a runtime target with the SDL2main library and the windows icon resource
function(HydrogenCore_SetupMain target)
    if(TARGET SDL2::SDL2main)
        target_link_libraries(${target} PRIVATE SDL2::SDL2main)
    endif()

    target_sources(${target} PRIVATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/icon/app.o)
endfunction()