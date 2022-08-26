if(DEFINED SYSTEM_REFERENCE_OPENCV_PATH)
    set(OpenCV_DIR "${SYSTEM_REFERENCE_OPENCV_PATH}/lib/cmake/opencv4/")
endif()
if (NOT DEFINED FPRIME_PRESCAN)
    find_package(OpenCV REQUIRED)
    set(OpenCV_STATIC ON)
endif()

#Ports
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ImagePort")
# Components
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Camera/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ImageProcessor/")
