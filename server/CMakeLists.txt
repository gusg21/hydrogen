
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_BINARY_DIR}>)

add_executable(HydrogenServer
        src/main.cc)


add_subdirectory(external/httplib)
set(HTTPLIB_INSTALL OFF)
set(HTTPLIB_NO_EXCEPTIONS ON)
target_link_libraries(HydrogenServer PUBLIC httplib::httplib)
set_target_properties(httplib PROPERTIES FOLDER "Vendored/httplib")

target_link_libraries(HydrogenServer PUBLIC HydrogenCore)

add_custom_target(HydrogenServer_Assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
set_target_properties(HydrogenServer_Assets PROPERTIES FOLDER "HyServerSupport")
add_dependencies(HydrogenServer HydrogenServer_Assets)