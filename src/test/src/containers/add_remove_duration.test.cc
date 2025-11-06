#include <gtest/gtest.h>

#include <nael_utils/containers/sorted_list.hh>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

namespace bpt = boost::posix_time;

TEST(add_remove_duration, tests)
{
    //empty list
    std::list<bpt::time_duration> durations;
    std::list<bpt::time_duration> ref_list;

    EXPECT_EQ(durations, ref_list) << "empty list";
    EXPECT_NO_THROW( remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(10), durations) ) << "removing from empty list should not throw";
    EXPECT_EQ(durations, ref_list) << "empty list";

    //Insert some elements
    add_element_in_sorted_list<bpt::time_duration>(bpt::minutes(30), durations);
    add_element_in_sorted_list<bpt::time_duration>(bpt::minutes(10), durations);
    add_element_in_sorted_list<bpt::time_duration>(bpt::minutes(20), durations);
    add_element_in_sorted_list<bpt::time_duration>(bpt::minutes(15), durations);
    add_element_in_sorted_list<bpt::time_duration>(bpt::minutes(10), durations);
    add_element_in_sorted_list<bpt::time_duration>(bpt::minutes(30), durations);
    add_element_in_sorted_list<bpt::time_duration>(bpt::minutes(15), durations);
    ref_list = {bpt::minutes(10), bpt::minutes(10), bpt::minutes(15), bpt::minutes(15), bpt::minutes(20), bpt::minutes(30), bpt::minutes(30)};
    EXPECT_EQ(durations, ref_list) << "resulting list should be sorted";

    //Remove elements one by one
    remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(15), durations);
    ref_list = {bpt::minutes(10), bpt::minutes(10), bpt::minutes(15), bpt::minutes(20), bpt::minutes(30), bpt::minutes(30)};
    EXPECT_EQ(durations, ref_list);

    remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(10), durations);
    ref_list = {bpt::minutes(10), bpt::minutes(15), bpt::minutes(20), bpt::minutes(30), bpt::minutes(30)};
    EXPECT_EQ(durations, ref_list);

    remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(30), durations);
    ref_list = {bpt::minutes(10), bpt::minutes(15), bpt::minutes(20), bpt::minutes(30)};
    EXPECT_EQ(durations, ref_list);

    remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(30), durations);
    ref_list = {bpt::minutes(10), bpt::minutes(15), bpt::minutes(20)};
    EXPECT_EQ(durations, ref_list);

    remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(15), durations);
    ref_list = {bpt::minutes(10), bpt::minutes(20)};
    EXPECT_EQ(durations, ref_list);
    remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(15), durations);
    EXPECT_EQ(durations, ref_list);

    remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(10), durations);
    ref_list = {bpt::minutes(20)};
    EXPECT_EQ(durations, ref_list);

    remove_element_from_sorted_list<bpt::time_duration>(bpt::minutes(20), durations);
    ref_list = {};
    EXPECT_EQ(durations, ref_list);

}

