#[=======================================================================[.rst:
valgrind
--------

This module defines functions to help use the valgrind tools.
:command:`enable_memcheck` enables rumtime leak detection.

Loading this file creates a ``run_memcheck`` target.

.. command:: enable_memcheck

  Automatically add memory leak detection with valgrind's memcheck for target.

  .. code-block:: cmake

    enable_memcheck(TARGET targetName
      [TARGET_ARGS args...]
      [MEMCHECK_ARGS args...]
      [EXCLUDE_FROM ALL]
    )

  ``enable_memcheck`` creates a new target ``run_memcheck_<targetName>`` to
  trigger analysis.

  The options are:

  ``TARGET targetName``
    Specifies the target to be analysed, which must be a known CMake executable
    target.

  ``TARGET_ARGS args...``
    Specifies additional arguments to be forwarded to the target called. This
    allows for instance to define a language folder, data directories, etc.

  ``MEMCHECK_ARGS args...``
    Specifies additional arguments to be appended to the memcheck call. This
    allows for instance to increase the stack trace size for some binaries.

  ``EXCLUDE_FROM_ALL``
    Normally, the function creates a dependency between the ``run_memcheck``
    target and the current ``run_memcheck_<targetName>`` target, allowing the
    first one to trigger the other one. If this behaviour is not desired (as may
    be the case when some resource is required but not always available), this
    option can be used to prevent the dependency from beeing added.

  For each registered target, a result file is created in
  ``${CMAKE_BINARY_DIR}/valgrind-reports``, the actual file name is
  ``<targetName>.memcheck-results.log``.

.. command:: run_memcheck

  Automatically call memcheck to run and analyse all registered
  ``run_memcheck_<target_name>`` targets (except those marked with
  ``EXCLUDE_FROM_ALL``).

Usage example:

  .. code-block:: cmake

    set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

    include(valgrind)

    add_library(FooLib FooLib.cc)
    add_executable(FooExe FooExe.cc)
    add_executable(FooTest FooTest.cc)

    enable_memcheck(TARGET FooTest
      TARGET_ARGS -lang "${CURRENT_SOURCE_DIR}/lang")

#]=======================================================================]

cmake_minimum_required (VERSION 3.16)
# 3.10 for include_guard
# 3.18 for find_program (REQUIRED)

include_guard (GLOBAL)

### ===============================================================================================
### Requirements
### ===============================================================================================

find_package (valgrind 3.9 REQUIRED)

### ===============================================================================================
### Automation
### ===============================================================================================

add_custom_target (run_memcheck
	COMMENT "Running valgrind memcheck memory management problem detection")

function (enable_memcheck)
	set(options EXCLUDE_FROM_ALL)
	set(oneValueArgs TARGET)
	set(multiValueArgs TARGET_ARGS MEMCHECK_ARGS)

	cmake_parse_arguments("" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	if (NOT _TARGET)
		message (FATAL_ERROR "TARGET is undefined")
	elseif (NOT TARGET ${_TARGET})
		message (FATAL_ERROR "TARGET ${_TARGET} must be a valid target")
	endif ()

	if (_UNPARSED_ARGUMENTS)
		message (FATAL_ERROR "unparsed arguments : ${_UNPARSED_ARGUMENTS}")
	endif ()

	set (valgrind_reports_path ${data_base_path}/valgrind-reports)
	file (MAKE_DIRECTORY "${valgrind_reports_path}")

	add_custom_target (run_memcheck_${_TARGET}
		COMMAND valgrind::valgrind --tool=memcheck
			--xml=yes --xml-file="${valgrind_reports_path}/${_TARGET}.memcheck-results.xml"
			--leak-check=full --show-leak-kinds=all
			--fair-sched=try
			${_MEMCHECK_ARGS}
			$<TARGET_FILE:${_TARGET}> ${_TARGET_ARGS}
		WORKING_DIRECTORY ${valgrind_reports_path}
		COMMENT "Running valgrind memcheck memory management problem detection (${_TARGET})")
	if (NOT _EXCLUDE_FROM_ALL)
		add_dependencies (run_memcheck run_memcheck_${_TARGET})
	endif ()
endfunction ()

