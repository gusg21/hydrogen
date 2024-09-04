cmake_minimum_required(VERSION 3.21)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}>)
if (DEFINED ANDROID)
    add_library(HydrogenRuntime SHARED src/main.cc)
    message(STATUS "(HydrogenRuntime) .so placed @ $<TARGET_FILE:HydrogenRuntime>")
else ()
    add_executable(HydrogenRuntime src/main.cc)
endif ()

target_link_libraries(HydrogenRuntime PUBLIC HydrogenCore)

if (ANDROID)
    add_custom_target(HydrogenRuntime_Assets
                      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_SOURCE_DIR}/android/hydrogen/app/src/main/assets/assets
    )
else ()
    add_custom_target(HydrogenRuntime_Assets
                      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
    )
endif ()
add_dependencies(HydrogenRuntime HydrogenRuntime_Assets)
set_target_properties(HydrogenRuntime_Assets PROPERTIES FOLDER "HyRuntimeSupport")

HydrogenCore_SetupMain(HydrogenRuntime)
HydrogenCore_CopyShaders(HydrogenRuntime ${CMAKE_CURRENT_BINARY_DIR})