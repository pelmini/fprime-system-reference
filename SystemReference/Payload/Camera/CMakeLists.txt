####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
#
# Note: using PROJECT_NAME as EXECUTABLE_NAME
####

set(SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Camera.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/Camera.cpp"
        )
register_fprime_module()

set(CMAKE_C_EXTENSIONS ON)
add_library(MockV4L2CameraLib "${CMAKE_CURRENT_LIST_DIR}/test/ut/CaptureMock.c")
# On Linux build the real library, otherwise use the mock implementation
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_library(V4L2CameraLib "${CMAKE_CURRENT_LIST_DIR}/Capture.c")
else()
	message(FATAL_ERROR "ERROR OCCURRED")
	add_library(V4L2CameraLib INTERFACE)
	target_link_libraries(MockV4L2CameraLib)
endif()
set(CMAKE_C_EXTENSIONS OFF)

# Register the unit test build
set(UT_SOURCE_FILES
	"${CMAKE_CURRENT_LIST_DIR}/Camera.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/TestMain.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/test/ut/Tester.cpp"
		)
set(UT_MOD_DEPS STest MockV4L2CameraLib)
register_fprime_ut()
