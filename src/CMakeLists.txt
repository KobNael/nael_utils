add_subdirectory (lib)
if(NOT ${PROJECT_NAME}_AS_SUBMODULE)
    add_subdirectory (test)
endif()
