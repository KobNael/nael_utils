# ============================================================================
# CMake configuration
# ============================================================================

cmake_minimum_required (VERSION 3.26)

# ============================================================================
# Project configuration
# ============================================================================

project (test_boost_desc
	VERSION 0.0
	LANGUAGES CXX)

# ============================================================================
# External requirements
# ============================================================================

# define dependent option definition helper
include (CMakeDependentOption)
# define package config generation helpers
include (CMakePackageConfigHelpers)
# define standardised install paths
include (GNUInstallDirs)

# ----------------------------------------------------------------------------
# External requirements setup
# ----------------------------------------------------------------------------
# additional cmake modules
list (PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# library behaviour
set (CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set (CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
set (CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# ============================================================================
# Compiler configurations
# ============================================================================

# ----------------------------------------------------------------------------
# Compilation settings
# ----------------------------------------------------------------------------
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------------------------------------------
# Default compilation flags
# ----------------------------------------------------------------------------
include (default_warnings)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps=obj")
# ----------------------------------------------------------------------------
# Enable cppcheck
# ----------------------------------------------------------------------------
option (${PROJECT_NAME}_WITH_CPPCHECK "enable cppcheck" OFF)
if (${PROJECT_NAME}_WITH_CPPCHECK)
	include (cppcheck)
	enable_cppcheck()
endif ()

# ----------------------------------------------------------------------------
# Coverage build type definition
# ----------------------------------------------------------------------------
option (${PROJECT_NAME}_WITH_COVERAGE "enable coverage" OFF)
if (${PROJECT_NAME}_WITH_COVERAGE)
	include (coverage)
endif ()

# ----------------------------------------------------------------------------
# Enable clang-tidy
# ----------------------------------------------------------------------------
cmake_dependent_option (${PROJECT_NAME}_WITH_CLANGTIDY "enable clang-tidy" OFF "NOT ${PROJECT_NAME}_WITH_COVERAGE" OFF)
if (${PROJECT_NAME}_WITH_CLANGTIDY)
	include (clangtidy)
endif ()

# ----------------------------------------------------------------------------
# Enable memcheck
# ----------------------------------------------------------------------------
option (${PROJECT_NAME}_WITH_VALGRIND "enable valgrind" OFF)
if (${PROJECT_NAME}_WITH_VALGRIND)
	include (valgrind)
endif ()

# ============================================================================
# Installation
# ============================================================================

#if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#	set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/${PROJECT_NAME}-${PROJECT_VERSION}" CACHE PATH "${PROJECT_NAME} default install path" FORCE)
#endif ()
#
#set (ConfigPackageLocation "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}")

# ----------------------------------------------------------------------------
# Configuration files
# ----------------------------------------------------------------------------
#write_basic_package_version_file ("${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config-version.cmake"
#	VERSION ${PROJECT_VERSION}
#	COMPATIBILITY SameMinorVersion)
#
#configure_package_config_file (cmake/${PROJECT_NAME}-config.cmake.in
#	"${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake"
#	INSTALL_DESTINATION "${ConfigPackageLocation}"
#	NO_SET_AND_CHECK_MACRO
#	NO_CHECK_REQUIRED_COMPONENTS_MACRO)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------
#install (FILES
#	"${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake"
#	"${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}-config-version.cmake"
#	DESTINATION "${ConfigPackageLocation}")

# ============================================================================
# Targets configurations
# ============================================================================

add_subdirectory (src)

# ============================================================================
# Documentation
# ============================================================================

#option (${PROJECT_NAME}_WITH_DOCUMENTATION "generate documentation" ON)
#if (${PROJECT_NAME}_WITH_DOCUMENTATION)
#	add_subdirectory (doc)
#endif ()
