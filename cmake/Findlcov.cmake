#[=======================================================================[.rst:
Findlcov
--------

this module looks for lcov



Once done this will define

::

  lcov_FOUND - system has lcov
  LCOV_EXECUTABLE - the lcov executable
  LCOV_VERSION_STRING - the version of lcov found



#]=======================================================================]

find_program (LCOV_EXECUTABLE
	NAMES lcov)

find_program (GENHTML_EXECUTABLE
	NAMES genhtml)

if (LCOV_EXECUTABLE)
	execute_process(COMMAND "${LCOV_EXECUTABLE}" --version
		OUTPUT_VARIABLE LCOV_OUTPUT_VARIABLE
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	string(REGEX REPLACE "^lcov: LCOV version ([0-9\\.]+)" "\\1" LCOV_VERSION_STRING "${LCOV_OUTPUT_VARIABLE}")
	unset(LCOV_OUTPUT_VARIABLE)
endif()

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (lcov
	REQUIRED_VARS LCOV_EXECUTABLE GENHTML_EXECUTABLE
	VERSION_VAR LCOV_VERSION_STRING)

mark_as_advanced (LCOV_EXECUTABLE GENHTML_EXECUTABLE)

if (lcov_FOUND AND NOT TARGET lcov::lcov)
	add_executable (lcov::lcov IMPORTED GLOBAL)
	set_target_properties (lcov::lcov
		PROPERTIES
		IMPORTED_LOCATION "${LCOV_EXECUTABLE}")

	add_executable (lcov::genhtml IMPORTED GLOBAL)
	set_target_properties (lcov::genhtml
		PROPERTIES
		IMPORTED_LOCATION "${GENHTML_EXECUTABLE}")
endif ()

