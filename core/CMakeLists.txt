cmake_minimum_required(VERSION 3.21)

set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)

# Core sources
set(HydrogenCore_Sources 

            # Core
            src/actorspecasset.cc include/core/actorspecasset.h
            src/asset.cc include/core/asset.h
            src/assets.cc include/core/assets.h
            src/engine.cc include/core/engine.h
            include/core/engineevents.h
            src/event.cc include/core/event.h
            src/eventqueue.cc include/core/eventqueue.h
            src/runtimeassets.cc include/core/runtimeassets.h
            src/runtimeengine.cc include/core/runtimeengine.h
            src/runtimesystem.cc include/core/runtimesystem.h
            src/runtimesystems.cc include/core/runtimesystems.h
            src/scene.cc include/core/scene.h
            src/scenespecasset.cc include/core/scenespecasset.h
            src/transform.cc include/core/transform.h
            src/window.cc include/core/window.h
            include/core/netrequestthreadcontext.h
            include/core/netrequestjob.h
            include/core/assetindex.h
            include/core/component.h
            include/core/widgets.h
            include/core/netrequestresult.h
            include/core/runtimeconsole.h
            src/runtimeconsole.cc
            include/core/log.h

            # Project
            src/project/project.cc include/core/project/project.h

            # Math
            src/math/mat4x4.cc include/core/math/mat4x4.h
            include/core/math/math.h
            src/math/quaternion.cc include/core/math/quaternion.h
            src/math/vector2.cc include/core/math/vector2.h
            src/math/vector3.cc include/core/math/vector3.h

            # Systems
            src/systems/gravity.cc include/core/systems/gravity.h
            src/systems/render/meshasset.cc include/core/systems/render/meshasset.h
            src/systems/render/renderer.cc include/core/systems/render/renderer.h
            src/systems/render/shader.cc include/core/systems/render/shader.h
            include/core/systems/render/vertex.h
            src/systems/script/scriptasset.cc include/core/systems/script/scriptasset.h
            src/systems/script/scriptcomp.cc include/core/systems/script/scriptcomp.h
            src/systems/script/scripting.cc include/core/systems/script/scripting.h
            src/systems/render/meshcomp.cc include/core/systems/render/meshcomp.h
            src/systems/render/gles3renderer.cc include/core/systems/render/gles3renderer.h
            src/systems/render/gl4renderer.cc include/core/systems/render/gl4renderer.h
            
            # Input
            include/core/input/inputactionsource.h
            include/core/input/keyinputactionsource.h
            src/input/input.cc include/core/input/input.h
            src/input/keyinputactionsource.cc 
            include/core/project/projectactionentry.h
            src/input/inputaction.cc include/core/input/inputaction.h
            src/input/dualkeyinputactionsource.cc include/core/input/dualkeyinputactionsource.h
)

add_library(HydrogenCore STATIC ${HydrogenCore_Sources}
        include/core/assetdescription.h)
add_library(HydrogenCore_Shared SHARED ${HydrogenCore_Sources})
if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:library")
endif ()
target_compile_definitions(HydrogenCore PRIVATE "HCORE_DEBUG=$<IF:$<CONFIG:Debug>,1,0>")
target_compile_definitions(HydrogenCore_Shared PRIVATE "HCORE_DEBUG=$<IF:$<CONFIG:Debug>,1,0>")

# Core include dir
target_include_directories(HydrogenCore PUBLIC include)
target_include_directories(HydrogenCore_Shared PUBLIC include)

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
target_link_libraries(HydrogenCore_Shared PUBLIC HydrogenCore_AngelScriptExtensions)

set_target_properties(angelscript PROPERTIES FOLDER "Vendored/AngelScript")
set_target_properties(HydrogenCore_AngelScriptExtensions PROPERTIES FOLDER "Vendored/AngelScript")

# Curl
set(ENABLE_CURL_MANUAL OFF)
set(BUILD_LIBCURL_DOCS OFF)
set(CURL_USE_LIBPSL OFF)
set(CURL_DISABLE_TESTS ON)
set(BUILD_CURL_EXE OFF)
set(CURL_DISABLE_INSTALL ON)
add_subdirectory(external/curl)
target_link_libraries(HydrogenCore PUBLIC libcurl)

set_target_properties(libcurl_static PROPERTIES FOLDER "Vendored/libcurl")
if (TARGET libcurl_object)
 set_target_properties(libcurl_object PROPERTIES FOLDER "Vendored/libcurl")
endif()
# SDL
#if (NOT ANDROID)
    set(SDL_STATIC ON)
#else ()
#    set(SDL_STATIC OFF)
#endif ()
set(SDL_SHARED ON) # not required but sometimes SDL will not build without it :(
set(SDL_DISABLE_UNINSTALL ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

if (TARGET SDL2::SDL2-static)
    message(STATUS "(HydrogenCore) Using SDL2 static")
    target_link_libraries(HydrogenCore PUBLIC SDL2::SDL2-static)
    target_link_libraries(HydrogenCore_Shared PUBLIC SDL2::SDL2-static)
    set_target_properties(SDL2-static PROPERTIES FOLDER "Vendored/SDL")
endif()
if (TARGET SDL2::SDL2-shared)
    message(STATUS "(HydrogenCore) Using SDL2 shared")
    target_link_libraries(HydrogenCore PUBLIC SDL2::SDL2-shared)
    target_link_libraries(HydrogenCore_Shared PUBLIC SDL2::SDL2-shared)
    set_target_properties(SDL2 PROPERTIES FOLDER "Vendored/SDL")
endif()
if (TARGET SDL2::SDL2main)
    target_link_libraries(HydrogenCore PRIVATE SDL2::SDL2main)
    target_link_libraries(HydrogenCore_Shared PRIVATE SDL2::SDL2main)
    set_target_properties(SDL2main PROPERTIES FOLDER "Vendored/SDL")
endif ()

set_target_properties(SDL2_test PROPERTIES FOLDER "Vendored/SDL")
set_target_properties(sdl_headers_copy PROPERTIES FOLDER "Vendored/SDL")
if (TARGET SDL2)
    set_target_properties(SDL2 PROPERTIES FOLDER "Vendored/SDL") # see above comment
endif ()

# OpenGL
if (DEFINED ANDROID)
    target_link_libraries(HydrogenCore PUBLIC GLESv3 ${log-lib})
    target_link_libraries(HydrogenCore_Shared PUBLIC GLESv3 ${log-lib})
else ()
    find_package(OpenGL REQUIRED)
    target_link_libraries(HydrogenCore PUBLIC ${OPENGL_LIBRARIES})
    target_link_libraries(HydrogenCore_Shared PUBLIC ${OPENGL_LIBRARIES})
endif ()

# GLM
#set(GLM_BUILD_TESTS OFF)
#set(BUILD_SHARED_LIBS OFF)
#add_subdirectory(external/glm)
#target_link_libraries(HydrogenCore PUBLIC glm::glm)
#set_target_properties(glm PROPERTIES FOLDER "Vendored/GLM")

# GLAD
add_subdirectory(external/glad)

target_link_libraries(HydrogenCore PUBLIC glad)
target_link_libraries(HydrogenCore_Shared PUBLIC glad)

set_target_properties(glad PROPERTIES FOLDER "Vendored/glad")

# ImGui integrations
add_subdirectory(external/imgui.cmake EXCLUDE_FROM_ALL)
target_link_libraries(HydrogenCore PUBLIC imgui.cmake)
target_include_directories(HydrogenCore PRIVATE external/sdl-imgui)
target_include_directories(HydrogenCore PRIVATE external/opengl-imgui)
target_link_libraries(HydrogenCore_Shared PUBLIC imgui.cmake)
target_include_directories(HydrogenCore_Shared PRIVATE external/sdl-imgui)
target_include_directories(HydrogenCore_Shared PRIVATE external/opengl-imgui)

target_sources(HydrogenCore PRIVATE
               external/sdl-imgui/imgui_impl_sdl2.cpp
               external/opengl-imgui/imgui_impl_opengl3.cpp
               )
target_sources(HydrogenCore_Shared PRIVATE
               external/sdl-imgui/imgui_impl_sdl2.cpp
               external/opengl-imgui/imgui_impl_opengl3.cpp
               )

set_target_properties(imgui.cmake PROPERTIES FOLDER "Vendored/imgui")

# tinygltf integration
set(TINYGLTF_INSTALL OFF)
set(TINYGLTF_HEADER_ONLY OFF)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)
add_subdirectory(external/tinygltf EXCLUDE_FROM_ALL)

target_link_libraries(HydrogenCore PUBLIC tinygltf)
target_link_libraries(HydrogenCore_Shared PUBLIC tinygltf)

set_target_properties(tinygltf PROPERTIES FOLDER "Vendored/tinygltf")

# yaml-cpp
set(YAML_BUILD_SHARED_LIBS OFF)
set(YAML_CPP_FORMAT_SOURCE OFF)
set(YAML_CPP_INSTALL OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(external/yaml-cpp EXCLUDE_FROM_ALL)

target_link_libraries(HydrogenCore PUBLIC yaml-cpp::yaml-cpp)
target_link_libraries(HydrogenCore_Shared PUBLIC yaml-cpp::yaml-cpp)

set_target_properties(yaml-cpp PROPERTIES FOLDER "Vendored/yaml-cpp")
set_target_properties(uninstall PROPERTIES FOLDER "Vendored/yaml-cpp/misc")

# generated shader code
function(HydrogenCore_CopyShaders target folder)
    add_custom_target(HydrogenCore_CopyShaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/hcore_assets
    )

    add_dependencies(${target} HydrogenCore_CopyShaders)

    set_target_properties(HydrogenCore_CopyShaders PROPERTIES FOLDER "HyCoreSupport")
endfunction()

# setup a runtime target with the SDL2main library and the windows icon resource
function(HydrogenCore_SetupMain target)
    if (TARGET SDL2::SDL2main)
        message(STATUS "(HydrogenCore_SetupMain) Linking SDL2main")
        target_link_libraries(${target} PUBLIC SDL2::SDL2main)
    endif ()

    if (MSVC)
        target_sources(${target} PRIVATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/icon/app.o)
    endif ()
endfunction()