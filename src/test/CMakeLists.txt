
if(NOT nael_utils_AS_SUBMODULE)
    include (unit_tests)
endif()

find_package (Boost REQUIRED)

target_sources (unit_tests
    PRIVATE
    #--------
    # utils
    #--------
    # computation utils
    src/computation/lcm.test.cc
    # dedicated exception
    src/exception/dedicated_exception.test.cc
    # containers
    src/containers/add_remove_duration.test.cc
    src/containers/sorted_list.test.cc
    # json utils
    src/json/stream_io.test.cc
    # log utils
    src/log/log_utils.test.cc
    # model utils
    src/model/model_desc.test.cc
    src/model/dto_to_bo.test.cc
    # period utils
    src/period/period_utils.test.cc
    src/period/capa_periods.test.cc
    src/period/periods.test.cc
    src/period/generate_periods.test.cc
    src/period/ratio_periods.test.cc
    src/piecewise_function/piecewise_function.test.cc
    # safe_comp utils
    src/safe_comp/safe_comp.test.cc
    # sort utils
    src/sort/lexical_sort.test.cc
    # string utils
    src/string/string_utils.test.cc
    # thread utils
    src/thread/thread_pool.test.cc
    #--------
    # helper for tests
    #--------
    src/test_data/test_data_helper.cc
    src/test_data/test_data_model.cc
    )

if (NOT nael_utils_AS_SUBMODULE AND nael_utils_WITH_COVERAGE)
    target_sources (unit_tests
        PRIVATE
        src/Utils_TestEnvironment.cc)
endif ()


target_link_libraries (unit_tests
    PRIVATE
        nael_utils
		Boost::boost
		Boost::json
        GTest::gtest_main
    )

target_include_directories (unit_tests
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    )

# ----------------------------------------------------------------------------
# run_unit_tests
# ----------------------------------------------------------------------------
if (NOT nael_utils_AS_SUBMODULE AND nael_utils_WITH_COVERAGE)
    generate_coverage_report (TARGET run_unit_tests)
endif ()

if (NOT nael_utils_AS_SUBMODULE AND nael_utils_WITH_VALGRIND)
    enable_memcheck (TARGET unit_tests)
endif ()


