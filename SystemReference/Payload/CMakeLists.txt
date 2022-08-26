set(OpenCV_DIR "${CMAKE_CURRENT_LIST_DIR}/build/opt/opencv-4.1.0/lib/cmake/opencv4/")
find_package(OpenCV REQUIRED)


#Ports
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ImagePort")
# Components
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Camera/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ImageProcessor/")
