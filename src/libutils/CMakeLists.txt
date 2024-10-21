find_package (Boost REQUIRED)

add_library(libutils)

set_target_properties (libutils
	PROPERTIES
		OUTPUT_NAME libutils
	)

target_sources (libutils
	PRIVATE
	src/utils/details/model_desc_print.cc
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

