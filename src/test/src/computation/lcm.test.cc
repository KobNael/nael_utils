#include <gtest/gtest.h>

#include <nael_utils/computation/lcm.hh>
#include <nael_utils/exception/exception.hh>

TEST(lcm, two_values)
{
    EXPECT_EQ(compute_lcm(4UL, 6UL), 12UL);
    EXPECT_EQ(compute_lcm(1UL, 1UL), 1UL);
    EXPECT_EQ(compute_lcm(7UL, 13UL), 91UL);
}

TEST(lcm, two_values_one_multiple_of_the_other)
{
    EXPECT_EQ(compute_lcm(3UL, 9UL), 9UL);
    EXPECT_EQ(compute_lcm(5UL, 5UL), 5UL);
}

TEST(lcm, two_values_throws_on_zero)
{
    EXPECT_THROW(compute_lcm(0UL, 5UL), fatal_error);
    EXPECT_THROW(compute_lcm(5UL, 0UL), fatal_error);
    EXPECT_THROW(compute_lcm(0UL, 0UL), fatal_error);
}

TEST(lcm, vector_single_value)
{
    EXPECT_EQ(compute_lcm(std::vector<unsigned>{7}), 7UL);
}

TEST(lcm, vector_multiple_values)
{
    EXPECT_EQ(compute_lcm(std::vector<unsigned>{4, 6}), 12UL);
    EXPECT_EQ(compute_lcm(std::vector<unsigned>{2, 3, 4}), 12UL);
    EXPECT_EQ(compute_lcm(std::vector<unsigned>{1, 1, 1}), 1UL);
}

TEST(lcm, vector_throws_on_empty)
{
    EXPECT_THROW(compute_lcm(std::vector<unsigned>{}), fatal_error);
}
