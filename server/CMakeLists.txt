add_library(HydrogenServer_Wrapper SHARED wrap/wrap.cc)
target_link_libraries(HydrogenServer_Wrapper PUBLIC HydrogenCore)

find_package(Python3 REQUIRED)
message(STATUS "(HydrogenServer) Have Python3 Executable: ${Python3_EXECUTABLE}")
#if (NOT DEFINED ${Python3_EXECUTABLE})
#    message(FATAL_ERROR "(HydrogenServer) No Python3 Executable specified.")
#endif()


add_custom_target(
        HydrogenServer
        ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/main.py $<TARGET_FILE:HydrogenServer_Wrapper>
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DEPENDS HydrogenServer_Wrapper
)