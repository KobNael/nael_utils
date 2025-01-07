#[=======================================================================[.rst:
Findgcov
--------

this module looks for gcov



Once done this will define

::

  gcov_FOUND - system has gcov
  GCOV_EXECUTABLE - the  executable
  GCOV_VERSION_STRING - the version of gcov found



#]=======================================================================]

get_filename_component (C_COMPILER_BIN_PATH "${CMAKE_C_COMPILER}" DIRECTORY)
get_filename_component (CXX_COMPILER_BIN_PATH "${CMAKE_CXX_COMPILER}" DIRECTORY)

find_program (GCOV_EXECUTABLE
	NAMES gcov
	HINTS "${C_COMPILER_BIN_PATH}" "${CXX_COMPILER_BIN_PATH}"
	NO_DEFAULT_PATH)

if (GCOV_EXECUTABLE)
	execute_process(COMMAND "${GCOV_EXECUTABLE}" --version
		OUTPUT_VARIABLE GCOV_OUTPUT_VARIABLE
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	string(REGEX REPLACE "^gcov( \\(GCC\\))? ([0-9\\.]+).*" "\\2" GCOV_VERSION_STRING "${GCOV_OUTPUT_VARIABLE}")
	unset(GCOV_OUTPUT_VARIABLE)
endif()

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (gcov
	REQUIRED_VARS GCOV_EXECUTABLE
	VERSION_VAR GCOV_VERSION_STRING)

mark_as_advanced (GCOV_EXECUTABLE)

if (gcov_FOUND AND NOT TARGET gcov::gcov)
	add_executable (gcov::gcov IMPORTED GLOBAL)
	set_target_properties (gcov::gcov
		PROPERTIES
		IMPORTED_LOCATION "${GCOV_EXECUTABLE}")
endif ()

