#[=======================================================================[.rst:
cppcheck
--------

This module defines functions to help use the cppcheck tool.
:command:`enable_cppcheck` enables parsing of the project source files.
:command:`run_cppcheck` triggers analysis and generates result file.

.. command:: enable_cppcheck

  Automatically add parsing with cppcheck.

    enable_cppcheck()

	 The source are retrieved from the compile commands database (see CMAKE_EXPORT_COMPILE_COMMANDS)

.. command:: run_cppcheck

  Automatically call cppcheck to parse the project source files and generate
  result file.
  The result file is created in ``${CMAKE_BINARY_DIR}/cppcheck-reports``,
  the actual file name is ``cppcheck-results.xml``.

Usage example:

  .. code-block:: cmake

    set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

	 include(cppcheck)
	 enable_cppcheck()

	 add_library(FooLib FooLib.cc)
    add_executable(FooExe FooExe.cc)
    add_executable(FooTest FooTest.cc)

#]=======================================================================]

cmake_minimum_required (VERSION 3.16)
# 3.10 for include_guard
# 3.12 for list (JOIN)

include_guard (GLOBAL)

### ===============================================================================================
### Requirements
### ===============================================================================================

find_package (cppcheck REQUIRED 1.76)
if (NOT cppcheck_FOUND)
	function (enable_cppcheck)
		add_custom_target (run_cppcheck
			COMMENT "Skipping cppcheck static analysis")
	endfunction ()

	return ()
endif ()

### ===============================================================================================
### Automation
### ===============================================================================================

function (enable_cppcheck)
	set (cppcheck_reports_path ${data_base_path}/cppcheck-reports)
	file (MAKE_DIRECTORY "${cppcheck_reports_path}")

	# note that ignore option (-i) is useless when using project mode (--project)
	# but, hell, we can hope some cppcheck future version will take it into account
	set (args
		"--enable=all"
		"--suppress=unmatchedSuppression"
		"--suppress=missingIncludeSystem"
		"--inline-suppr"
		"--xml"
		"--xml-version=2"
		"-i" "*.test.cc"
		"-i" "*.test.cpp")

	if (EXISTS "${PROJECT_SOURCE_DIR}/cppcheck.suppr")
		list (APPEND args
			"--suppressions-list=${PROJECT_SOURCE_DIR}/cppcheck.supp")
	endif ()

	list (JOIN args " " args_string)

	add_custom_target (run_cppcheck
		COMMAND cppcheck::cppcheck --project=${data_base_path}/compile_commands.json ${args} 2> cppcheck-results.xml
		WORKING_DIRECTORY "${cppcheck_reports_path}"
		COMMENT "Running cppcheck static analysis"
		VERBATIM
		COMMAND_EXPAND_LISTS)
endfunction ()

