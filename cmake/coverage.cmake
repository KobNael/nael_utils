#[=======================================================================[.rst:
coverage
--------

This module defines functions to help get the code coeverage reports.
:command:`enable_coverage` adds required compile and link options to the target,
:command:`generate_coverage_report` adds dependency on target to generate the
coverage target, :command:`run_coverage` triggers code coverage generation.

Coverage reports use gcovr (REQUIRED) and lcov (OPTIONAL) to process the
coverage data and generate reports.

.. command:: enable_coverage

  Automatically add compile and link options to the target::

    enable_coverage(TARGET target)

  The option is:

  ``TARGET target``
    Specifies target, which must be a known CMake library or executable target.

.. command:: generate_coverage_report

  Automatically add dependency to the target::

    generate_coverage_report(TARGET target)

  The option is:

  ``TARGET target``
    Specifies target, which must be a known CMake library or executable target.
    When :command:`run_coverage` is called, the targets that had been passed to
    :command:`generate_coverage_report` are triggered to ensure coverage data
    is available.

.. command:: generate_coverage_reports

  Deprecated version of `generate_coverage_report`. Please replace with `generate_coverage_report`.

.. command:: run_coverage

  Automatically trigger targets generating coverage data, then the tools for
  generating the reports.

  The reports are created in ``${CMAKE_BINARY_DIR}/coverage-reports``.

  For gcovr, report is named ``gcov-results.xml``.
  If gcovr version is at least 4.2, a HTML report is also generated in
  ``${CMAKE_BINARY_DIR}/coverage-reports/gcovr``, with main file ``index.html``.

  For lcov, report is named ``lcov.info``.
  A HTML report is also generated in
  ``${CMAKE_BINARY_DIR}/coverage-reports/lcov``, with main file ``index.html``.

Usage example:

  .. code-block:: cmake

    include(coverage)
    add_library(FooLib FooLib.cc)

    enable_coverage(TARGET FooLib)
    add_executable(FooExe FooExe.cc)

    enable_coverage(TARGET FooExe)
    add_executable(FooTest FooTest.cc)
     
    generate_coverage_report(TARGET FooTest)

#]=======================================================================]

cmake_minimum_required (VERSION 3.16)
# 3.10 for include_guard

include_guard (GLOBAL)

### ===============================================================================================
### Requirements
### ===============================================================================================

set (skip_coverage ON)
if (MSVC)
	message (WARNING "coverage called under MSVC, skipped...")
elseif (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
	message (WARNING "coverage called in non Debug mode, skipped...")
else ()
	set (skip_coverage OFF)
endif ()

if (skip_coverage)
	add_custom_target (run_coverage
		COMMENT "Generate coverage reports : skipped")

	function (enable_coverage)
	endfunction ()

	function (generate_coverage_report)
	endfunction ()

	return ()
endif ()

### -----------------------------------------------------------------------------------------------
### gcov
### -----------------------------------------------------------------------------------------------
find_package (gcov REQUIRED)

### -----------------------------------------------------------------------------------------------
### gcovr
### -----------------------------------------------------------------------------------------------
find_package (gcovr REQUIRED)

### -----------------------------------------------------------------------------------------------
### lcov
### -----------------------------------------------------------------------------------------------
find_package (lcov 1.14)

### ===============================================================================================
### Automation
### ===============================================================================================

set (coverage_reports_path "${PROJECT_BINARY_DIR}/coverage-reports")

### -----------------------------------------------------------------------------------------------
### coverage compiler and linker flags
### -----------------------------------------------------------------------------------------------
# set coverage compiler flags
list (APPEND COVERAGE_CFLAGS -g --coverage)
if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	include (CheckCXXCompilerFlag)
	check_cxx_compiler_flag (-fprofile-abs-path HAVE_fprofile_abs_path)
	if (HAVE_fprofile_abs_path)
		list (APPEND COVERAGE_CFLAGS
			-fprofile-abs-path
			#[[ end of list ]])
	endif ()
	check_cxx_compiler_flag (-fprofile-update=atomic HAVE_fprofile_update_atomic)
	if (HAVE_fprofile_update_atomic)
		list (APPEND COVERAGE_CFLAGS
			-fprofile-update=atomic
			#[[ end of list ]])
	endif ()
endif ()

mark_as_advanced (COVERAGE_CFLAGS)

### -----------------------------------------------------------------------------------------------
### set gcovr options
### -----------------------------------------------------------------------------------------------
if (gcovr_FOUND)
	list (APPEND GCOVR_CMD
		--gcov-executable "${GCOV_EXECUTABLE}"
		--exclude ".*/src/.*\\.test\\..*"
		--exclude ".*/tests/src/.*"
		--exclude ".*/unit_tests/src/.*"
		--exclude ".*/func_tests/src/.*"
		--exclude-unreachable-branches
		--xml --xml-pretty
		--output "${coverage_reports_path}/gcov-results.xml"
		--root "${PROJECT_SOURCE_DIR}"
		--object-directory "${PROJECT_BINARY_DIR}"
		"${PROJECT_BINARY_DIR}"
		#[[ end of list ]])

	if (GCOVR_VERSION_STRING VERSION_GREATER_EQUAL "7")
		message (STATUS "coverage: adding Gcovr 7.0+ options")

		list (APPEND GCOVR_CMD
			--txt-metric line
			--sort uncovered-percent
			#[[ end of list ]])
	elseif (GCOVR_VERSION_STRING VERSION_GREATER_EQUAL "4.2")
		message (STATUS "coverage: adding Gcovr 4.2+ options")
		list (APPEND GCOVR_CMD
			--sort-percentage
			#[[ end of list ]])
	endif ()
	if (GCOVR_VERSION_STRING VERSION_GREATER_EQUAL "4.2")
		list (APPEND GCOVR_CMD
			--exclude-throw-branches
			--html --html-details "${coverage_reports_path}/gcovr/index.html"
			--sonarqube "${coverage_reports_path}/sonarqube.xml"
			#[[ end of list ]])
	endif ()

	if (GCOVR_VERSION_STRING VERSION_GREATER_EQUAL "4.2")
		include (ProcessorCount)
		ProcessorCount (NPROC)
		if (${NPROC} GREATER 20)
			set (NPROC 20)
		endif ()
		if (NPROC EQUAL 0)
			message (WARNING "Unable to determine number of cores")
			set (NPROC 1)
		endif ()

		message (STATUS "coverage: using ${NPROC} threads for gcovr report generation")

		list (APPEND GCOVR_CMD
			-j ${NPROC})
	endif ()
endif ()

### -----------------------------------------------------------------------------------------------
### set lcov options
### -----------------------------------------------------------------------------------------------
if (lcov_FOUND)
	# LCOV command line
	list (APPEND LCOV_CMD
		--gcov-tool "${GCOV_EXECUTABLE}"
		--quiet
		--capture
		--no-external
		--ignore-errors unused
		--exclude "*/src/*.test.*"
		--exclude "*/tests/src/*"
		--exclude "*/unit_tests/src/*"
		--exclude "*/func_tests/src/*"
		--base-directory "${PROJECT_SOURCE_DIR}"
		--directory "${PROJECT_BINARY_DIR}"
		--output-file "${coverage_reports_path}/lcov.info"
		#[[ end of list ]])

	# genhtml command line
	list (APPEND GENHTML_CMD $<TARGET_FILE:lcov::genhtml>
		"${coverage_reports_path}/lcov.info"
		--quiet
		--sort
		--output-directory "${coverage_reports_path}/lcov"
		#[[ end of list ]])
endif ()

### -----------------------------------------------------------------------------------------------
### add target
### -----------------------------------------------------------------------------------------------
add_custom_target (run_coverage
	COMMENT "Generate coverage reports")

if (gcovr_FOUND)
	add_custom_target (run_coverage_gcovr
		COMMAND ${CMAKE_COMMAND} -E make_directory "${coverage_reports_path}/gcovr"
		COMMAND gcovr::gcovr ${GCOVR_CMD}
		COMMENT "- generate gcovr reports"
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		VERBATIM
		COMMAND_EXPAND_LISTS)
endif ()

add_dependencies (run_coverage run_coverage_gcovr)

if (lcov_FOUND)
	add_custom_target (run_coverage_lcov
		COMMAND ${CMAKE_COMMAND} -E make_directory "${coverage_reports_path}/lcov"
		COMMAND lcov::lcov ${LCOV_CMD}
		COMMAND ${GENHTML_CMD}
		COMMENT "- generate lcov reports"
		VERBATIM
		COMMAND_EXPAND_LISTS)

	add_dependencies (run_coverage run_coverage_lcov)
endif ()

### -----------------------------------------------------------------------------------------------
### enable coverage for target
### -----------------------------------------------------------------------------------------------
function (enable_coverage)
	set(options )
	set(oneValueArgs TARGET)
	set(multiValueArgs )

	cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if (NOT _TARGET)
		message (FATAL_ERROR "TARGET is undefined")
	elseif (NOT TARGET ${_TARGET})
		message (FATAL_ERROR "TARGET ${_TARGET} must be a valid target")
	endif ()

	if (_UNPARSED_ARGUMENTS)
		message (FATAL_ERROR "unparsed arguments : ${_UNPARSED_ARGUMENTS}")
	endif ()

	foreach (CFLAG ${COVERAGE_CFLAGS})
		target_compile_options (${_TARGET} PRIVATE ${CFLAG})
	endforeach ()

	target_link_options (${_TARGET} PUBLIC --coverage)
endfunction ()

### -----------------------------------------------------------------------------------------------
### generate coverage report
### -----------------------------------------------------------------------------------------------
function (generate_coverage_report)
	set(options )
	set(oneValueArgs TARGET)
	set(multiValueArgs )

	cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if (NOT _TARGET)
		message (FATAL_ERROR "TARGET is undefined")
	elseif (NOT TARGET ${_TARGET})
		message (FATAL_ERROR "TARGET ${_TARGET} must be a valid target")
	endif ()

	message (STATUS "enable coverage for TARGET ${_TARGET}")

	if (gcovr_FOUND)
		add_dependencies (run_coverage_gcovr ${_TARGET})
	endif ()

	if (lcov_FOUND)
		add_dependencies (run_coverage_lcov ${_TARGET})
	endif ()
endfunction ()

