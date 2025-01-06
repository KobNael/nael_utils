# Automagically set-up compilation flags for Eurodecision projects
#
# Usage :
# In the project CMakeLists.txt, adds the following line :
#
#    include (SetCompilFlags)
#
# One can later update these flags if needed
#

cmake_minimum_required (VERSION 3.15)

message (DEPRECATION "Deprecated. Please replace with _default_warnings_, and/or standard CMake and conan calls.")

# Generates compilation commands in dedicated file
# The file can then be use with other tools
# cppcheck for instance (cppcheck --project compile_commands.json)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Microsoft Visual C++ compiler
if (MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

	# Default to statically-linked runtime.
	option (STATIC_RUNTIME "Use statically-linked (MT) or dynamically-linked (MD) MSVC runtime" OFF)

	if (STATIC_RUNTIME)
		message (STATUS "Use MSVC statically-linked runtime (MT).")
	else ()
		message (STATUS "Use MSVC dynamically-linked runtime (MD).")
	endif ()

	set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<IF:$<BOOL:${STATIC_RUNTIME}>,,DLL>")

# GNU g++ compiler
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options (-Wall
		-Wextra
		-pedantic)

# LLVM clang++ compiler
elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_compile_options (-Weverything
		-Wno-missing-prototypes
		-Wno-padded
		-Wno-float-equal
		-Wno-documentation-unknown-command
		-Wno-unknown-warning-option)

# Unidentified compiler, failure
else ()
	message (FATAL_ERROR "Error : unable to identify compiler")

endif ()

