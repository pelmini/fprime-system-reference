find_package(OpenCV REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/opencv/opt/opencv-4.1.0")
#Ports
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ImagePort")
# Components
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Camera/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ImageProcessor/")