####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and auto coding files
# MOD_DEPS: (optional) module dependencies
#
# Note: using PROJECT_NAME as EXECUTABLE_NAME
####
set(SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/PhotoConverter.fpp"
        "${CMAKE_CURRENT_LIST_DIR}/PhotoConverter.cpp"
        )
register_fprime_module()