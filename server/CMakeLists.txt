add_library(HydrogenServer_Wrapper SHARED wrap/wrap.cc)
target_link_libraries(HydrogenServer_Wrapper PUBLIC HydrogenCore)

add_custom_target(
        HydrogenServer
        ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/main.py $<TARGET_FILE:HydrogenServer_Wrapper>
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DEPENDS HydrogenServer_Wrapper
)