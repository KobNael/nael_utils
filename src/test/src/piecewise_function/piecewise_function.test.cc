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
    EXPECT_TRUE(safecomp::eq(segment.get_slope(), 1.0));
    EXPECT_TRUE(safecomp::eq(segment.get_y(1.0), 1.0));
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
    EXPECT_TRUE(safecomp::eq(segment.get_slope(), -1.0));
    EXPECT_TRUE(safecomp::eq(segment.get_y(1.0), -1.0));
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

TEST(piecewise_function, in_range)
{
    Segment segment{{-1.0, -1.0}, {1.0, 1.0}};
    ASSERT_TRUE(segment.x_in_range(0.0));
    ASSERT_FALSE(segment.x_in_range(-2.0));
    ASSERT_FALSE(segment.x_in_range(2.0));
    ASSERT_TRUE(segment.y_in_range(0.0));
    ASSERT_FALSE(segment.y_in_range(-2.0));
    ASSERT_FALSE(segment.y_in_range(2.0));
    ASSERT_TRUE(segment.y_in_range(1.0));
    segment = {{-1.0, 1.0}, {1.0, -1.0}};
    ASSERT_TRUE(segment.x_in_range(0.0));
    ASSERT_FALSE(segment.x_in_range(-2.0));
    ASSERT_FALSE(segment.x_in_range(2.0));
    ASSERT_TRUE(segment.y_in_range(0.0));
    ASSERT_FALSE(segment.y_in_range(-2.0));
    ASSERT_FALSE(segment.y_in_range(2.0));
    ASSERT_TRUE(segment.y_in_range(1.0));
    segment = {{1.0, 1.0}, {10.0, 1.0}};
    ASSERT_TRUE(segment.x_in_range(1.0));
    ASSERT_TRUE(segment.x_in_range(1.0));
    ASSERT_TRUE(segment.y_in_range(1.0));
    ASSERT_FALSE(segment.y_in_range(2.0));
}

TEST(piecewise_function, get_lowest_dot)
{
    Piecewise_linear_function pwf = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 310.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 80.0));
    ASSERT_TRUE(safecomp::eq(res._y, -220.0));
    // on horizontal segment
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 10.0));
    ASSERT_TRUE(safecomp::eq(res._x, 00.0));
    ASSERT_TRUE(safecomp::eq(res._y, 100.0));
    // on random segment
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 79.0));
    ASSERT_TRUE(safecomp::eq(res._x, 79.0));
    ASSERT_TRUE(safecomp::eq(res._y, -212.6666667));
    // on exact dot
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 15.0, 200.0));
    ASSERT_TRUE(safecomp::eq(res._x, 80.0));
    ASSERT_TRUE(safecomp::eq(res._y, -220.0));
    // out of range
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 200.0, 300.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, -200.0, -10.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    // on bounds
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, -100.0, 0.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0));
    ASSERT_TRUE(safecomp::eq(res._y, 100.0));
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 100.0, 300.0));
    ASSERT_TRUE(safecomp::eq(res._x, 100.0));
    ASSERT_TRUE(safecomp::eq(res._y, 180.0));
}

TEST(piecewise_function, get_highest_dot)
{
    Piecewise_linear_function pwf = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 310.0}, {20.0, 220.0}, {21.0, 270.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 0.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 10.0));
    ASSERT_TRUE(safecomp::eq(res._y, 310.0));
    // on horizontal segment
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 0.0, 8.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0));
    ASSERT_TRUE(safecomp::eq(res._y, 100.0));
    // on random segment
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 15.0, 20.0));
    ASSERT_TRUE(safecomp::eq(res._x, 15.0));
    ASSERT_TRUE(safecomp::eq(res._y, 265.));
    // on exact dot
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 15.0, 80.0));
    ASSERT_TRUE(safecomp::eq(res._x, 21.0));
    ASSERT_TRUE(safecomp::eq(res._y, 270.0));
    // out of range
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 200.0, 300.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_highest_dot(pwf, -200.0, -10.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    // on bounds
    ASSERT_NO_THROW(res = get_highest_dot(pwf, -100.0, 0.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0));
    ASSERT_TRUE(safecomp::eq(res._y, 100.0));
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 100.0, 300.0));
    ASSERT_TRUE(safecomp::eq(res._x, 100.0));
    ASSERT_TRUE(safecomp::eq(res._y, 300.0));
}

TEST(piecewise_function, get_first_dot_above)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 110.0}, {10.0, 110.0}, {10.0, 310.0},
        {20.0, 220.0}, {21.0, 270.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 50., 0.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0));
    ASSERT_TRUE(safecomp::eq(res._y, 50.0));
    // on horizontal segment
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 110, 0.0, 20.0));
    ASSERT_TRUE(safecomp::eq(res._x, 5.0));
    ASSERT_TRUE(safecomp::eq(res._y, 110.0));
    // on random segment
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 245., 20.0, 22.0));
    ASSERT_TRUE(safecomp::eq(res._x, 20.5));
    ASSERT_TRUE(safecomp::eq(res._y, 245.));
    // on exact dot
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 310., .0, 120.0));
    ASSERT_TRUE(safecomp::eq(res._x, 10.0));
    ASSERT_TRUE(safecomp::eq(res._y, 310.0));
    // out of range
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 10., 200.0, 300.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 30., -200.0, -10.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 400., -200.0, 400.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    // on bounds
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 0., -100.0, 0.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0));
    ASSERT_TRUE(safecomp::eq(res._y, 0.0));
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 300., 100.0, 300.0));
    ASSERT_TRUE(safecomp::eq(res._x, 100.0));
    ASSERT_TRUE(safecomp::eq(res._y, 300.0));
}

TEST(piecewise_function, get_last_dot_above)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 320.0}, {10.0, 320.0}, {10.0, 310.0},
        {20.0, 325.0}, {21.0, 270.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 250., 0.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 100.0));
    ASSERT_TRUE(safecomp::eq(res._y, 300.0));
    // on horizontal segment
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 320, 0.0, 10.0));
    ASSERT_TRUE(safecomp::eq(res._x, 10.0));
    ASSERT_TRUE(safecomp::eq(res._y, 320.0));
    // on random segment
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 317.5, 10.0, 15.0));
    ASSERT_TRUE(safecomp::eq(res._x, 15.));
    ASSERT_TRUE(safecomp::eq(res._y, 317.5));
    // on exact dot
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 325., .0, 120.0));
    ASSERT_TRUE(safecomp::eq(res._x, 20.0));
    ASSERT_TRUE(safecomp::eq(res._y, 325.0));
    // out of range
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 10., 200.0, 300.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 30., -200.0, -10.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 400., -200.0, 400.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    // on bounds
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 0., -100.0, 0.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0));
    ASSERT_TRUE(safecomp::eq(res._y, 100.0));
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 300., 100.0, 300.0));
    ASSERT_TRUE(safecomp::eq(res._x, 100.0));
    ASSERT_TRUE(safecomp::eq(res._y, 300.0));
}

TEST(piecewise_function, get_first_dot_below)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 320.0}, {10.0, 320.0}, {10.0, 310.0},
        {20.0, -180.0}, {21.0, -220.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 150.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 250., 0.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0));
    ASSERT_TRUE(safecomp::eq(res._y, 0.0));
    // on horizontal segment
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, -220, 21.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 21.0));
    ASSERT_TRUE(safecomp::eq(res._y, -220.0));
    // on random segment
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, -200., 20.0, 21.0));
    ASSERT_TRUE(safecomp::eq(res._x, 20.5));
    ASSERT_TRUE(safecomp::eq(res._y, -200.));
    // on exact dot
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, -220., .0, 120.0));
    ASSERT_TRUE(safecomp::eq(res._x, 21.0));
    ASSERT_TRUE(safecomp::eq(res._y, -220.0));
    // out of range
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 10., 200.0, 300.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 30., -200.0, -10.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, -400., -200.0, 400.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    // on bounds
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 10., -100.0, 0.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 150., 100.0, 300.0));
    ASSERT_TRUE(safecomp::eq(res._x, 100.0));
    ASSERT_TRUE(safecomp::eq(res._y, 150.0));
    // special case : vertical segment invalid
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 0., -100.0, 150.0));
    Segment segment{{10.0, 310.0}, {20.0, -180.0}};
    ASSERT_TRUE(safecomp::eq(res._x, segment.get_x(0.0)));
    ASSERT_TRUE(safecomp::eq(res._y, 0.0));
}

TEST(piecewise_function, get_last_dot_below)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 320.0}, {10.0, 320.0}, {10.0, 310.0},
        {20.0, -180.0}, {21.0, -220.0}, {80.0, -220.0}, {80.0, 90.0}, {90.0, 90.0}, {100.0, 180.0}, {100.0, 150.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 160., 0.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 100.0));
    ASSERT_TRUE(safecomp::eq(res._y, 150.0));
    // on horizontal segment
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 90, 0.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 90.0));
    ASSERT_TRUE(safecomp::eq(res._y, 90.0));
    // on random segment
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 135., 90.0, 100.0));
    ASSERT_TRUE(safecomp::eq(res._x, 95.));
    ASSERT_TRUE(safecomp::eq(res._y, 135.));
    // on exact dot
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, -220., .0, 120.0));
    ASSERT_TRUE(safecomp::eq(res._x, 80.0));
    ASSERT_TRUE(safecomp::eq(res._y, -220.0));
    // out of range
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 10., 200.0, 300.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 30., -200.0, -10.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, -400., -200.0, 400.0));
    ASSERT_TRUE(std::isnan(res._x));
    ASSERT_TRUE(std::isnan(res._y));
    // on bounds
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 0., -100.0, 0.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0));
    ASSERT_TRUE(safecomp::eq(res._y, 0.0));
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 150., 100.0, 300.0));
    ASSERT_TRUE(safecomp::eq(res._x, 100.0));
    ASSERT_TRUE(safecomp::eq(res._y, 150.0));
}
