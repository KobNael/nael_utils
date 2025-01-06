find_package (Boost REQUIRED)

add_library(libutils)

set_target_properties (libutils
	PROPERTIES
		OUTPUT_NAME libutils
	)

target_sources (libutils
	PRIVATE
	src/utils/model/details/model_streamable.cc
	src/utils/log/log.cc
	src/utils/log/Logger.cc
	src/utils/period/period_utils.cc
	src/utils/string/string_utils.cc
	)

set_default_warnings (TARGET libutils)

target_include_directories (libutils
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

target_link_libraries (libutils
	PUBLIC
		Boost::boost
		Boost::json
	)

