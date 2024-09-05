add_library(HydrogenServer_Wrapper SHARED wrap/wrap.cc)
target_link_libraries(HydrogenServer_Wrapper PUBLIC HydrogenCore)

add_custom_target(HydrogenServer_Assets
                      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets $<TARGET_FILE:HydrogenServer_Wrapper>/assets
    )
# add_dependencies(HydrogenServer_Wrapper HydrogenServer_Assets)