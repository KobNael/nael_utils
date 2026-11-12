#[=======================================================================[.rst:
default_warnings
----------------

This module defines functions to help define standardized compiler warning and
error messages.

:command:`set_default_warnings` enables compiler warning and error messages.

.. command:: set_default_warnings

  Automatically set compiler warning and error messages for target.

  set_default_warnings(TARGET target)

  The option is:

  ``TARGET target``
    Specifies target, which must be a known CMake library or executable target.


Usage example:

  .. code-block:: cmake

	 include(default_warnings)
    
	 add_library(FooLib FooLib.cc)
	 set_default_warnings(TARGET FooLib)
    
	 add_executable(FooExe FooExe.cc)
	 set_default_warnings(TARGET FooExe)

    add_executable(FooTest FooTest.cc)

#]=======================================================================]

cmake_minimum_required (VERSION 3.16)
# 3.10 for include_guard

include_guard (GLOBAL)

### ===============================================================================================
### Requirements
### ===============================================================================================

### ===============================================================================================
### Automation
### ===============================================================================================

if (MSVC)
	function (set_default_warnings)
	endfunction ()

	return ()
elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	function (set_default_warnings)
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

		target_compile_options (${_TARGET}
		PRIVATE
			-Wall
			# implies :
			# -Waddress
			# -Warray-bounds=1 # (only with -O2)
			# -Warray-compare
			# -Warray-parameter=2 # (C and Objective-C only)
			# -Wbool-compare
			# -Wbool-operation
			# -Wc++11-compat
			# -Wc++14-compat
			# -Wcatch-value # (C++ and Objective-C++ only)
			# -Wchar-subscripts
			# -Wcomment
			# -Wduplicate-decl-specifier # (C and Objective-C only)
			# -Wenum-compare # (in C/ObjC; this is on by default in C++)
			# -Wformat
			# -Wformat-overflow
			# -Wformat-truncation
			# -Wint-in-bool-context
			# -Wimplicit # (C and Objective-C only)
			# -Wimplicit-int # (C and Objective-C only)
			# -Wimplicit-function-declaration # (C and Objective-C only)
			# -Winit-self # (only for C++)
			# -Wlogical-not-parentheses
			# -Wmain # (only for C/ObjC and unless -ffreestanding)
			# -Wmaybe-uninitialized
			# -Wmemset-elt-size
			# -Wmemset-transposed-args
			# -Wmisleading-indentation # (only for C/C++)
			# -Wmismatched-dealloc
			# -Wmismatched-new-delete # (only for C/C++)
			# -Wmissing-attributes
			# -Wmissing-braces # (only for C/ObjC)
			# -Wmultistatement-macros
			# -Wnarrowing # (only for C++)
			# -Wnonnull
			# -Wnonnull-compare
			# -Wopenmp-simd
			# -Wparentheses
			# -Wpessimizing-move # (only for C++)
			# -Wpointer-sign # (only for C/ObjC)
			# -Wrange-loop-construct # (only for C++)
			# -Wreorder
			# -Wrestrict
			# -Werror=return-type
			# -Wsequence-point
			# -Wsign-compare # (only in C++)
			# -Wsizeof-array-div
			# -Wsizeof-pointer-div
			# -Wsizeof-pointer-memaccess
			# -Wstrict-aliasing
			# -Wstrict-overflow=1
			# -Werror=switch
			# -Wtautological-compare
			# -Wtrigraphs
			# -Werror=uninitialized
			# -Wno-error=maybe-uninitialized
			# -Wunknown-pragmas
			# -Werror=unused-function
			# -Werror=unused-label
			# -Werror=unused-value
			# -Werror=unused-variable
			# -Wvla-parameter # (C and Objective-C only)
			# -Wvolatile-register-var
			# -Wzero-length-bounds

			-Wextra
			# implies :
			# -Wclobbered
			# -Wcast-function-type
			# -Wdeprecated-copy # (C++ only)
			# -Wempty-body
			# -Wenum-conversion # (C only)
			# -Wignored-qualifiers
			# -Wimplicit-fallthrough=3
			# -Wmissing-field-initializers
			# -Wmissing-parameter-type # (C only)
			# -Wold-style-declaration # (C only)
			# -Woverride-init # (C and Objective-C only)
			# -Wsign-compare # (C only)
			# -Wstring-compare
			# -Wredundant-move # (only for C++)
			# -Wtype-limits
			# -Werror=uninitialized
			# -Wno-error=maybe-uninitialized
			# -Wshift-negative-value # (in C++03 and in C99 and newer)
			# -Wunused-parameter # (only with -Wunused or -Wall)
			# -Wunused-but-set-parameter # (only with -Wunused or -Wall)

			# other
			-Werror=shadow
			-Werror=deprecated
			-Wdeprecated-declarations

		#[[end of list ]])
	endfunction ()
endif ()

