cmake_minimum_required(VERSION 3.21)
project(Hydrogen LANGUAGES CXX C)
set (CMAKE_CXX_STANDARD 17)

set(ALLOW_DUPLICATE_CUSTOM_TARGETS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set(HYCORE_HEADLESS ON)

add_subdirectory(core) # core library of common components

#add_subdirectory(editor) # editor GUI
add_subdirectory(runtime) # runtime code
#add_subdirectory(server) # server code, specifically core library generation