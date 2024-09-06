cmake_minimum_required(VERSION 3.21)

add_executable(HydrogenEditor src/main.cc)

target_link_libraries(HydrogenEditor PUBLIC HydrogenCore)

HydrogenCore_SetupMain(HydrogenEditor)
HydrogenCore_CopyShaders(HydrogenEditor ${CMAKE_CURRENT_BINARY_DIR})