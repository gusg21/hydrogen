cmake_minimum_required(VERSION 3.21)

add_executable(HydrogenEditor src/main.cc
        src/editor.cc
        include/editor/editor.h
        include/editor/editorwindow.h
        src/editorwindow.cc
        include/editor/windows/meshimporter.h
        src/windows/meshimporter.cc
        include/editor/windows/projectexplorer.h
        src/windows/projectexplorer.cc)

target_include_directories(HydrogenEditor PUBLIC include)

target_link_libraries(HydrogenEditor PUBLIC HydrogenCore)

HydrogenCore_SetupMain(HydrogenEditor)
HydrogenCore_CopyShaders(HydrogenEditor ${CMAKE_CURRENT_BINARY_DIR})