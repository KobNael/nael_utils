#include <gtest/gtest.h>

#include <nael_utils/piecewise_function/piecewise_function.hh>
#include <nael_utils/safe_comp/safe_comp.hh>

TEST(piecewise_function, segment_utils)
{
    Segment segment{{-1.0, -1.0}, {1.0, 1.0}};
    EXPECT_TRUE(segment.contains({0.5, 0.5}));
    EXPECT_TRUE(segment.contains({0.0, 0.0}));
    EXPECT_FALSE(segment.contains({-2.0, -2.0}));
    EXPECT_FALSE(segment.contains({2.0, 2.0}));
    EXPECT_FALSE(segment.contains({0.5, 0.6}));
    EXPECT_FALSE(std::isnan(segment.get_slope()));
    EXPECT_TRUE(safecomp::eq(segment.get_slope(), 1.0L));
    EXPECT_TRUE(safecomp::eq(segment.get_y(1.0), 1.0L));
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    ASSERT_DEBUG_DEATH(segment.get_y(2.0), ".*Assertion `safecomp::le\\(_from._x, x\\) && safecomp::le\\(x, _to._x\\)' failed.*");
    ASSERT_DEBUG_DEATH(segment.get_y(-12.), ".*Assertion `safecomp::le\\(_from._x, x\\) && safecomp::le\\(x, _to._x\\)' failed.*");

    segment = {{-1.0, 1.0}, {1.0, -1.0}};
    EXPECT_TRUE(segment.contains({0.5, -0.5}));
    EXPECT_TRUE(segment.contains({0.0, 0.0}));
    EXPECT_FALSE(segment.contains({-2.0, -2.0}));
    EXPECT_FALSE(segment.contains({2.0, 2.0}));
    EXPECT_FALSE(segment.contains({0.5, 0.6}));
    EXPECT_FALSE(std::isnan(segment.get_slope()));
    EXPECT_TRUE(safecomp::eq(segment.get_slope(), -1.0L));
    EXPECT_TRUE(safecomp::eq(segment.get_y(1.0), -1.0L));
    ASSERT_DEBUG_DEATH(segment.get_y(2.0), ".*Assertion `safecomp::le\\(_from._x, x\\) && safecomp::le\\(x, _to._x\\)' failed.*");
    ASSERT_DEBUG_DEATH(segment.get_y(-12.), ".*Assertion `safecomp::le\\(_from._x, x\\) && safecomp::le\\(x, _to._x\\)' failed.*");

    segment = {{1.0, 0.0}, {1.0, 1.0}};
    EXPECT_TRUE(segment.contains({1.0, 0.5}));
    EXPECT_FALSE(segment.contains({1.0, 1.5}));
    EXPECT_FALSE(segment.contains({1.5, 0.5}));
    EXPECT_TRUE(std::isnan(segment.get_slope()));
    EXPECT_TRUE(std::isnan(segment.get_y(1.0)));
    ASSERT_DEBUG_DEATH(segment.get_y(2.0), ".*Assertion `safecomp::le\\(_from._x, x\\) && safecomp::le\\(x, _to._x\\)' failed.*");
    ASSERT_DEBUG_DEATH(segment.get_y(-12.), ".*Assertion `safecomp::le\\(_from._x, x\\) && safecomp::le\\(x, _to._x\\)' failed.*");
    EXPECT_TRUE(std::isnan(segment.get_y(1.0)));

}

extern void merge(Piecewise_linear_function &pwf);

TEST(piecewise_function, merge_segments)
{
    Piecewise_linear_function fn = {{-1.0, -1.0}, {0.0, 0.0}, {2.5, 2.5}, {2.6, 2.6}, {4.0, 4.0}};
    Piecewise_linear_function expected = {{-1.0, -1.0}, {4.0, 4.0}};
    ASSERT_NO_THROW(merge(fn));
    ASSERT_EQ(fn, expected);

    fn = {{-1.0, -1.0}, {0.0, 0.0}, {2.5, 2.5}, {2.5, 2.6}, {4.0, 4.0}};
    expected = {{-1.0, -1.0}, {2.5, 2.5}, {2.5, 2.6}, {4.0, 4.0}};
    ASSERT_NO_THROW(merge(fn));
    ASSERT_EQ(fn, expected);

    fn = {{-1.0, -1.0}, {0.0, -1.0}, {0.0, 2.5}, {2.5, 2.5}, {4.0, 2.5}};
    expected = {{-1.0, -1.0}, {0.0, -1.0}, {0.0, 2.5}, {4.0, 2.5}};
    ASSERT_NO_THROW(merge(fn));
    ASSERT_EQ(fn, expected);

    fn = {{-1.0, -1.0}, {0.0, -1.0}, {0.0, 1.0}, {0.0, 2.5}, {2.5, 2.5}, {4.0, 2.5}};
    expected = {{-1.0, -1.0}, {0.0, -1.0}, {0.0, 2.5}, {4.0, 2.5}};
    ASSERT_NO_THROW(merge(fn));
    ASSERT_EQ(fn, expected);
}



TEST(piecewise_function, add_variation)
{
    // init with a flat function (y=100)
    Piecewise_linear_function fn = {{0.0, 100.0}, {100.0, 100.0}};

    // add a variation +50 at x=10
    Segment variation{{10.0, 0.0}, {10.0, 50.0}};
    Piecewise_linear_function expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {100.0, 150.0}};
    Piecewise_linear_function result;
    ASSERT_NO_THROW(result = add_variation(fn, variation));
    ASSERT_EQ(result, expected);

    // add a variation +70 from x=10 to x=20 to the previous result
    variation = {{10.0, 0.0}, {20.0, 70.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {100.0, 220.0}};
    ASSERT_NO_THROW(result = add_variation(result, variation));
    ASSERT_EQ(result, expected);

    // add a variation +80 at x=100
    variation = {{100.0, 0.0}, {100.0, 80.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {100.0, 220.0}, {100.0, 300.0}};
    ASSERT_NO_THROW(result = add_variation(result, variation));
    ASSERT_EQ(result, expected);

    // add a variation -100 at x=0
    variation = {{0.0, 0.0}, {0.0, -100.0}};
    expected = {{0.0, 100.0}, {0.0, 0.0}, {10.0, 0.0}, {10.0, 50.0}, {20.0, 120.0}, {100.0, 120.0}, {100.0, 200.0}};
    ASSERT_NO_THROW(result = add_variation(result, variation));
    ASSERT_EQ(result, expected);
    // add a variation +100 at x=0
    variation = {{0.0, 0.0}, {0.0, 100.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {100.0, 220.0}, {100.0, 300.0}};
    ASSERT_NO_THROW(result = add_variation(result, variation));
    ASSERT_EQ(result, expected);
    // add a variation -50 at x=0
    variation = {{0.0, 0.0}, {0.0, -50.0}};
    expected = {{0.0, 100.0}, {0.0, 50.0}, {10.0, 50.0}, {10.0, 100.0}, {20.0, 170.0}, {100.0, 170.0}, {100.0, 250.0}};
    ASSERT_NO_THROW(result = add_variation(result, variation));
    ASSERT_EQ(result, expected);
    // add a variation +50 at x=100
    variation = {{100.0, 0.0}, {100.0, 50.0}};
    expected = {{0.0, 100.0}, {0.0, 50.0}, {10.0, 50.0}, {10.0, 100.0}, {20.0, 170.0}, {100.0, 170.0}, {100.0, 300.0}};
    ASSERT_NO_THROW(result = add_variation(result, variation));
    ASSERT_EQ(result, expected);
    // Add again +50 at x=0
    variation = {{0.0, 0.0}, {0.0, 50.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {100.0, 220.0}, {100.0, 350.0}};
    ASSERT_NO_THROW(result = add_variation(result, variation));
    ASSERT_EQ(result, expected);
    // Remove 40 at x=80
    variation = {{80.0, 0.0}, {80.0, -40.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {80.0, 220.0}, {80.0, 180.0}, {100.0, 180.0}, {100.0, 310.0}};
    ASSERT_NO_THROW(result = add_variation(result, variation));
    ASSERT_EQ(result, expected);
}