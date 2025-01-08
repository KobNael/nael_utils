#[=======================================================================[.rst:
Findcppcheck
------------

this module looks for cppcheck



Once done this will define

::

  CPPCHECK_FOUND - system has cppcheck
  CPPCHECK_EXECUTABLE - the cppcheck executable
  CPPCHECK_VERSION_STRING - the version of cppcheck found



#]=======================================================================]

find_program (CPPCHECK_EXECUTABLE
	NAMES cppcheck
	HINTS ${cppcheck_ROOT}
	PATH_SUFFIXES bin
	DOC "cppcheck static analysis program path")

if (CPPCHECK_EXECUTABLE)
	execute_process (COMMAND "${CPPCHECK_EXECUTABLE}" --version
		OUTPUT_VARIABLE CPPCHECK_OUTPUT_VARIABLE
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	string (REGEX REPLACE "Cppcheck ([0-9.]+)$" "\\1" CPPCHECK_VERSION_STRING "${CPPCHECK_OUTPUT_VARIABLE}")
	unset(CPPCHECK_OUTPUT_VARIABLE)
endif()

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (cppcheck
	REQUIRED_VARS CPPCHECK_EXECUTABLE
	VERSION_VAR CPPCHECK_VERSION_STRING)

mark_as_advanced (CPPCHECK_EXECUTABLE)

if (cppcheck_FOUND AND NOT TARGET cppcheck::cppcheck)
	add_executable (cppcheck::cppcheck IMPORTED GLOBAL)
	set_target_properties (cppcheck::cppcheck
		PROPERTIES
		IMPORTED_LOCATION "${CPPCHECK_EXECUTABLE}")
endif ()

