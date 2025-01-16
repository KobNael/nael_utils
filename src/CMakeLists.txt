add_subdirectory (nael_utils)
if(NOT ${PROJECT_NAME}_AS_SUBMODULE)
    add_subdirectory (test)
endif()
