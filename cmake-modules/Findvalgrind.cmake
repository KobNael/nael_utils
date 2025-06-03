#[=======================================================================[.rst:
Findvalgrind
------------

this module looks for valgrind



Once done this will define

::

  valgrind_FOUND - system has valgrind
  VALGRIND_EXECUTABLE - the valgrind executable
  VALGRIND_VERSION_STRING - the version of valgrind found



#]=======================================================================]

find_program (VALGRIND_EXECUTABLE
	NAMES valgrind
	HINTS ${valgrind_ROOT}
	PATH_SUFFIXES bin
	DOC "valgrind memory program path")

if (VALGRIND_EXECUTABLE)
	execute_process (COMMAND "${VALGRIND_EXECUTABLE}" --version
		OUTPUT_VARIABLE VALGRIND_OUTPUT_VARIABLE
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	string (REGEX REPLACE "valgrind-([0-9.]+).*$" "\\1" VALGRIND_VERSION_STRING "${VALGRIND_OUTPUT_VARIABLE}")
	unset(VALGRIND_OUTPUT_VARIABLE)
endif()

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (valgrind
	REQUIRED_VARS VALGRIND_EXECUTABLE
	VERSION_VAR VALGRIND_VERSION_STRING)

mark_as_advanced (VALGRIND_EXECUTABLE)

if (valgrind_FOUND AND NOT TARGET valgrind::valgrind)
	add_executable (valgrind::valgrind IMPORTED GLOBAL)
	set_target_properties (valgrind::valgrind
		PROPERTIES
		IMPORTED_LOCATION "${VALGRIND_EXECUTABLE}")
endif ()

