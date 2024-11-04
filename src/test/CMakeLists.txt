
include (unit_tests)

target_sources (unit_tests
	PRIVATE
	src/capa_periods.test.cc
	src/periods.test.cc
	src/simple.test.cc
    src/thread_pool.test.cc
)

target_link_libraries (unit_tests
	PRIVATE
		libutils
		GTest::gtest_main
	)

# ----------------------------------------------------------------------------
# run_unit_tests
# ----------------------------------------------------------------------------
if (${PROJECT_NAME}_WITH_COVERAGE)
	generate_coverage_report (TARGET run_unit_tests)
endif ()




