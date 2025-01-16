# ============================================================================
# External requirements
# ============================================================================

# ----------------------------------------------------------------------------
# Boost
# ----------------------------------------------------------------------------
find_package (Boost REQUIRED)

# ============================================================================
# Targets configuration
# ============================================================================

configure_file (${CMAKE_SOURCE_DIR}/cmake-modules/nael_version.hh.in ${CMAKE_CURRENT_BINARY_DIR}/nael_utils_version.hh)

# ----------------------------------------------------------------------------
# library configuration
# ----------------------------------------------------------------------------

add_library(nael_utils)

if (NOT ${PROJECT_NAME}_AS_SUBMODULE AND ${PROJECT_NAME}_WITH_COVERAGE)
	enable_coverage(TARGET nael_utils)
endif()

set_target_properties (nael_utils
	PROPERTIES
		OUTPUT_NAME nael_utils
	)

target_sources (nael_utils
	PRIVATE
	src/nael_utils/exception/exception.cc
	src/nael_utils/log/log.cc
	src/nael_utils/log/Logger.cc
	src/nael_utils/period/period_functor.cc
	src/nael_utils/period/period_utils.cc
	src/nael_utils/string/string_utils.cc
	)

set_default_warnings (TARGET nael_utils)

target_include_directories (nael_utils
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

target_link_libraries (nael_utils
	PUBLIC
		Boost::boost
		Boost::json
	)

# ============================================================================
# Install configuration
# ============================================================================

install (TARGETS nael_utils EXPORT nael_utils-export
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (DIRECTORY src/nael_utils
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING
		PATTERN "*.hh"
		PATTERN "*.tpp"
)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/nael_utils_version.hh
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (EXPORT nael_utils-export
	FILE ${PROJECT_NAME}-targets.cmake
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION "${ConfigPackageLocation}"
)

# ----------------------------------------------------------------------------
# Export configuration
# ----------------------------------------------------------------------------
add_library (${PROJECT_NAME}::nael_utils ALIAS nael_utils)

