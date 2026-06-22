#include <gtest/gtest.h>

#include <nael_utils/containers/sorted_list.hh>

TEST(sorted_list, add_element_keeps_order_with_duplicates)
{
    std::list<int> values;

    add_element_in_sorted_list(4, values);
    add_element_in_sorted_list(1, values);
    add_element_in_sorted_list(3, values);
    add_element_in_sorted_list(2, values);
    add_element_in_sorted_list(2, values);
    add_element_in_sorted_list(5, values);

    std::list<int> expected {1, 2, 2, 3, 4, 5};
    EXPECT_EQ(values, expected);
}

TEST(sorted_list, remove_element_removes_single_occurrence)
{
    std::list<int> values {1, 2, 2, 3};

    remove_element_from_sorted_list(2, values);

    std::list<int> expected_after_first_remove {1, 2, 3};
    EXPECT_EQ(values, expected_after_first_remove);

    remove_element_from_sorted_list(4, values);
    EXPECT_EQ(values, expected_after_first_remove);

    remove_element_from_sorted_list(2, values);
    std::list<int> expected_after_second_remove {1, 3};
    EXPECT_EQ(values, expected_after_second_remove);
}

TEST(sorted_list, intersect_returns_false_for_empty_lists)
{
    std::list<int> values1;
    std::list<int> values2;

    EXPECT_FALSE(intersect(values1, values2));
}

TEST(sorted_list, intersect_returns_true_when_common_value_exists)
{
    std::list<int> values1 {1, 3, 5, 8};
    std::list<int> values2 {0, 2, 5, 7};

    EXPECT_TRUE(intersect(values1, values2));
    EXPECT_TRUE(intersect(values2, values1));
}

TEST(sorted_list, intersect_returns_false_for_disjoint_lists)
{
    std::list<int> values1 {1, 3, 5};
    std::list<int> values2 {2, 4, 6};

    EXPECT_FALSE(intersect(values1, values2));
}
