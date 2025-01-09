
include (unit_tests)

target_sources (unit_tests
	PRIVATE
	# json utils
		#todo
	# log utils
		#todo
	# model utils
		#todo
	# period utils
	src/period/capa_periods.test.cc
	src/period/periods.test.cc
	# safe_comp utils
		#todo
	# sort utils
	src/sort/lexical_sort.test.cc
	# string utils
	src/string/string_utils.test.cc
	# thread utils
    src/thread/thread_pool.test.cc
)

target_link_libraries (unit_tests
	PRIVATE
		nael_utils
		GTest::gtest_main
	)

# ----------------------------------------------------------------------------
# run_unit_tests
# ----------------------------------------------------------------------------
if (${PROJECT_NAME}_WITH_COVERAGE)
	generate_coverage_report (TARGET run_unit_tests)
endif ()

if (${PROJECT_NAME}_WITH_VALGRIND)
	enable_memcheck (TARGET unit_tests)
endif ()


