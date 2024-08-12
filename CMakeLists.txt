cmake_minimum_required(VERSION 3.21)
project(Hydrogen LANGUAGES CXX)

add_subdirectory(core) # core library of common components
add_subdirectory(editor) # editor GUI
add_subdirectory(runtime) # runtime code