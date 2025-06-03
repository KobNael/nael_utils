#[=======================================================================[.rst:
Findgcovr
---------

this module looks for gcovr



Once done this will define

::

  gcovr_FOUND - system has gcovr
  GCOVR_EXECUTABLE - the gcovr executable
  GCOVR_VERSION_STRING - the version of gcovr found



#]=======================================================================]

find_program (GCOVR_EXECUTABLE
	NAMES gcovr)

if (GCOVR_EXECUTABLE)
	execute_process(COMMAND "${GCOVR_EXECUTABLE}" --version
		OUTPUT_VARIABLE GCOVR_OUTPUT_VARIABLE
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	string(REGEX REPLACE "^gcovr ([0-9\\.]+).*" "\\1" GCOVR_VERSION_STRING "${GCOVR_OUTPUT_VARIABLE}")
	unset(GCOVR_OUTPUT_VARIABLE)
endif()

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (gcovr
	REQUIRED_VARS GCOVR_EXECUTABLE
	VERSION_VAR GCOVR_VERSION_STRING)

mark_as_advanced (GCOVR_EXECUTABLE)

if (gcovr_FOUND AND NOT TARGET gcovr::gcovr)
	add_executable (gcovr::gcovr IMPORTED GLOBAL)
	set_target_properties (gcovr::gcovr
		PROPERTIES
		IMPORTED_LOCATION "${GCOVR_EXECUTABLE}")
endif ()

