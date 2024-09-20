find_package (Boost REQUIRED COMPONENTS program_options)

add_executable (boost_desc)

set_target_properties (boost_desc
	PROPERTIES
		OUTPUT_NAME boost_desc
	)

target_sources (boost_desc
	PRIVATE
	src/main.cc
	src/utils/model_desc_print.cc
	)

set_default_warnings (TARGET boost_desc)

target_include_directories (boost_desc
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

target_link_libraries (boost_desc
	PRIVATE
		Boost::boost
		Boost::json
		Boost::program_options
	)

