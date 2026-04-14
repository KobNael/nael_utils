#include <gtest/gtest.h>

#include <nael_utils/piecewise_function/piecewise_function.hh>
#include <nael_utils/safe_comp/safe_comp.hh>
#include <nael_utils/log/log.hh>

TEST(piecewise_function, segment_utils)
{
    // Segment [(-1,-1), (1,1)[
    Segment segment{{-1.0, -1.0}, {1.0, 1.0}};
    // on the segment
    EXPECT_TRUE(segment.contains({0.5, 0.5}));
    EXPECT_TRUE(segment.contains({0.0, 0.0}));
    // on the straight line but not on the segment
    EXPECT_FALSE(segment.contains({-2.0, -2.0}));
    EXPECT_FALSE(segment.contains({2.0, 2.0}));
    // not even on the straight line
    EXPECT_FALSE(segment.contains({0.5, 0.6}));
    // test slope
    EXPECT_FALSE(std::isnan(segment.get_slope()));
    EXPECT_TRUE(safecomp::eq(segment.get_slope(), 1.0));
    EXPECT_FALSE(segment.is_vertical());
    EXPECT_FALSE(segment.is_horizontal());
    // get y valid
    EXPECT_TRUE(safecomp::eq(segment.get_y(.9), 0.9));
    EXPECT_TRUE(safecomp::eq(segment.get_y(1.0), 1.0));
    // get y out of bound
    EXPECT_TRUE(safecomp::eq(segment.get_y(2.0),2.0));
    EXPECT_TRUE(safecomp::eq(segment.get_y(-12.0),-12.0));

    // Segment [(-1,1), (1,-1)[
    segment = {{-1.0, 1.0}, {1.0, -1.0}};
    // on the segment
    EXPECT_TRUE(segment.contains({0.5, -0.5}));
    EXPECT_TRUE(segment.contains({0.0, 0.0}));
    // on the straight line but not on the segment
    EXPECT_FALSE(segment.contains({-2.0, -2.0}));
    EXPECT_FALSE(segment.contains({2.0, 2.0}));
    // not even on the straight line
    EXPECT_FALSE(segment.contains({0.5, 0.6}));
    // test slope
    EXPECT_FALSE(std::isnan(segment.get_slope()));
    EXPECT_TRUE(safecomp::eq(segment.get_slope(), -1.0));
    EXPECT_FALSE(segment.is_vertical());
    EXPECT_FALSE(segment.is_horizontal());
    // get y valid
    EXPECT_TRUE(safecomp::eq(segment.get_y(0.0), 0.0));
    // last dot IS not out of bound for computation
    EXPECT_TRUE(safecomp::eq(segment.get_y(1.0), -1.0));
    // get y out of bound
    EXPECT_TRUE(safecomp::eq(segment.get_y(2.0), -2.0));
    EXPECT_TRUE(safecomp::eq(segment.get_y(12.0), -12.0));

    // vertical segment [(1,0), (1,1)[
    segment = {{1.0, 0.0}, {1.0, 1.0}};
    // on the segment
    EXPECT_TRUE(segment.contains({1.0, 0.5}));
    // on the straight line but not on the segment
    EXPECT_FALSE(segment.contains({1.0, 1.5}));
    // not even on the straight line
    EXPECT_FALSE(segment.contains({1.5, 0.5}));
    // vertical segment => slope is nan
    EXPECT_TRUE(segment.is_vertical());
    EXPECT_FALSE(segment.is_horizontal());
    EXPECT_TRUE(std::isnan(segment.get_slope()));
    // get y is nan for vertical segment
    EXPECT_TRUE(std::isnan(segment.get_y(1.0)));
    EXPECT_TRUE(std::isnan(segment.get_y(.5)));

    // horizontal segment [(0,1), (1,1)[
    segment = {{0.0, 1.0}, {1.0, 1.0}};
    // on the segment
    EXPECT_TRUE(segment.contains({0.5, 1.0}));
    // on the straight line but not on the segment
    EXPECT_FALSE(segment.contains({1.5, 1.0}));
    // not even on the straight line
    EXPECT_FALSE(segment.contains({1., 0.5}));
    // horizontal segment => slope is 0
    EXPECT_FALSE(segment.is_vertical());
    EXPECT_TRUE(segment.is_horizontal());
    EXPECT_TRUE(safecomp::eq(segment.get_slope(), 0.0));
    // get y is valid for horizontal segment
    EXPECT_TRUE(safecomp::eq(segment.get_y(0.5), 1.0));
    EXPECT_TRUE(safecomp::eq(segment.get_y(0.0), 1.0));
    EXPECT_TRUE(safecomp::eq(segment.get_y(2.0), 1.0));
    EXPECT_TRUE(safecomp::eq(segment.get_y(-12.0), 1.0));
}

TEST(piecewise_function, multiply)
{
    Piecewise_linear_function fn = {{0.0, 100.0}, {100.0, 100.0}};
    Piecewise_linear_function expected = {{0.0, 200.0}, {100.0, 200.0}};
    Piecewise_linear_function result;
    EXPECT_NO_THROW(result = multiply(fn, 2.0));
    EXPECT_EQ(result, expected);

    expected = {{0.0, 50.0}, {100.0, 50.0}};
    EXPECT_NO_THROW(result = multiply(fn, 0.5));
    EXPECT_EQ(result, expected);

    // factor must be non null
    EXPECT_THROW(multiply(fn, 0.0), std::invalid_argument);
}

TEST(piecewise_function, get_y)
{
    Piecewise_linear_function fn = {{0.0, 100.0}, {10.0, 200.0}, {20.0, 150.0}};
    // in the bounds
    auto [y, y_second] = get_y(fn, 5.0);
    EXPECT_TRUE(safecomp::eq(y, 150.0));
    EXPECT_TRUE(std::isnan(y_second));
    // on a dot
    std::tie(y, y_second) = get_y(fn, 10.0);
    EXPECT_TRUE(safecomp::eq(y, 200.0));
    EXPECT_TRUE(std::isnan(y_second));
    // out of bounds
    std::tie(y, y_second) = get_y(fn, -1.0);
    EXPECT_TRUE(std::isnan(y));
    EXPECT_TRUE(std::isnan(y_second));
    std::tie(y, y_second) = get_y(fn, 21.0);
    EXPECT_TRUE(std::isnan(y));
    EXPECT_TRUE(std::isnan(y_second));

    // vertical segment
    fn = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 200.0}, {20.0, 150.0}};
    std::tie(y, y_second) = get_y(fn, 10.0);
    EXPECT_TRUE(safecomp::eq(y, 100.0)) << y;
    EXPECT_TRUE(safecomp::eq(y_second, 200.0)) << y_second;
}

TEST(piecewise_function, get_lower_envelope)
{
    Piecewise_linear_function fn1 = {{0.0, 100.0}, {10.0, 200.0}, {20.0, 150.0}};
    Piecewise_linear_function fn2 = {{0.0, 150.0}, {10.0, 150.0}, {20.0, 150.0}};
    Piecewise_linear_function expected = {{0.0, 100.0}, {10.0, 150.0}, {20.0, 150.0}};
    Piecewise_linear_function result;
    EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
    EXPECT_EQ(result, expected);

}

TEST(piecewise_function, get_lower_envelope_with_vertical_segments)
{
    Piecewise_linear_function result;

    // Test case 1: First function has vertical segment, second doesn't
    {
        Piecewise_linear_function fn1 = {
            {0.0, 50.0},
            {10.0, 100.0}, {10.0, 200.0},
            {20.0, 150.0}};
        Piecewise_linear_function fn2 = {
            {0.0, 80.0},
            {10.0, 120.0},
            {20.0, 160.0}};
        Piecewise_linear_function expected = {
            {0.0, 50.0},
            {10.0, 100.0}, {10.0, 120.0},
            {20.0, 150.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 2: Second function has vertical segment, first doesn't
    {
        Piecewise_linear_function fn1 = {
            {0.0, 80.0},
            {10.0, 120.0},
            {20.0, 160.0}};
        Piecewise_linear_function fn2 = {
            {0.0, 50.0},
            {10.0, 100.0}, {10.0, 200.0},
            {20.0, 150.0}};
        Piecewise_linear_function expected = {
            {0.0, 50.0},
            {10.0, 100.0}, {10.0, 120.0},
            {20.0, 150.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 3: Both functions have vertical segments at same x-coordinate
    {
        Piecewise_linear_function fn1 = {
            {0.0, 50.0},
            {10.0, 80.0}, {10.0, 120.0},
            {20.0, 150.0}};
        Piecewise_linear_function fn2 = {
            {0.0, 60.0},
            {10.0, 90.0}, {10.0, 110.0},
            {20.0, 140.0}};
        Piecewise_linear_function expected = {
            {0.0, 50.0},
            {10.0, 80.0}, {10.0, 110.0},
            {20.0, 140.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 4: Both functions have vertical segments at different x-coordinates
    {
        Piecewise_linear_function fn1 = {
            {0.0, 50.0},
            {5.0, 60.0}, {5.0, 120.0},
            {15.0, 130.0},
            {20.0, 140.0}};
        Piecewise_linear_function fn2 = {
            {0.0, 70.0},
            {10.0, 80.0}, {10.0, 110.0},
            {15.0, 115.0},
            {20.0, 120.0}};
        Piecewise_linear_function expected = {
            {0.0, 50.0},
            {5.0, 60.0}, {5.0, 75.0},
            {10.0, 80.0}, {10.0, 110.0},
            {15.0, 115.0},
            {20.0, 120.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 5: Vertical segment at the beginning
    {
        Piecewise_linear_function fn1 = {
            {0.0, 80.0}, {0.0, 120.0},
            {10.0, 150.0},
            {20.0, 180.0}};
        Piecewise_linear_function fn2 = {
            {0.0, 100.0},
            {10.0, 130.0},
            {20.0, 160.0}};
        Piecewise_linear_function expected = {
            {0.0, 80.0}, {0.0, 100.0},
            {10.0, 130.0},
            {20.0, 160.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 6: Vertical segment at the end
    {
        Piecewise_linear_function fn1 = {
            {0.0, 100.0},
            {10.0, 120.0},
            {20.0, 140.0},
            {20.0, 200.0}};
        Piecewise_linear_function fn2 = {
            {0.0, 90.0},
            {10.0, 110.0},
            {20.0, 130.0}};
        Piecewise_linear_function expected = {
            {0.0, 90.0},
            {10.0, 110.0},
            {20.0, 130.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 7: Multiple vertical segments in both functions
    {
        Piecewise_linear_function fn1 = {
            {0.0, 60.0}, {0.0, 80.0},
            {5.0, 90.0}, {5.0, 110.0},
            {15.0, 120.0}, {15.0, 140.0},
            {20.0, 150.0}};
        Piecewise_linear_function fn2 = {
            {0.0, 70.0},
            {5.0, 85.0}, {5.0, 95.0},
            {10.0, 100.0}, {10.0, 130.0},
            {15.0, 135.0},
            {20.0, 145.0}};
        Piecewise_linear_function expected = {
            {0.0, 60.0}, {0.0, 70.0},
            {5.0, 85.0}, {5.0, 95.0},
            {10.0, 100.0}, {10.0, 115.0},
            {15.0, 120.0}, {15.0, 135.0},
            {20.0, 145.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 8: Vertical segments with different orientations (up and down)
    {
        Piecewise_linear_function fn1 = {
            {0.0, 100.0},
            {10.0, 150.0}, {10.0, 80.0},
            {20.0, 120.0}};  // Drop at x=10
        Piecewise_linear_function fn2 = {
            {0.0, 90.0},
            {10.0, 70.0}, {10.0, 140.0},
            {20.0, 160.0}};   // Jump at x=10
        Piecewise_linear_function expected = {
            {0.0, 90.0},
            {10.0, 70.0}, {10.0, 80.0},
            {20.0, 120.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 9: Overlapping vertical segments with same x but different y ranges
    {
        Piecewise_linear_function fn1 = {
            {0.0, 50.0},
            {10.0, 100.0}, {10.0, 200.0},
            {20.0, 150.0}};  // Vertical from 100 to 200
        Piecewise_linear_function fn2 = {
            {0.0, 60.0},
            {10.0, 120.0}, {10.0, 180.0},
            {20.0, 160.0}};  // Vertical from 120 to 180
        Piecewise_linear_function expected = {
            {0.0, 50.0},
            {10.0, 100.0}, {10.0, 180.0},
            {20.0, 150.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 10: One function entirely vertical, other normal
    {
        Piecewise_linear_function fn1 = {
            {10.0, 50.0},
            {10.0, 150.0}};  // Entirely vertical
        Piecewise_linear_function fn2 = {
            {0.0, 80.0},
            {10.0, 120.0},
            {20.0, 160.0}};
        Piecewise_linear_function expected = {
            {0.0, 80.0},
            {10.0, 50.0}, {10.0, 120.0},
            {20.0, 160.0}};  // Only the intersection point
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 11: Vertical segments creating discontinuities
    {
        Piecewise_linear_function fn1 = {
            {0.0, 100.0},
            {5.0, 50.0}, {5.0, 150.0},
            {10.0, 200.0}};   // Jump up at x=5
        Piecewise_linear_function fn2 = {
            {0.0, 80.0},
            {5.0, 120.0}, {5.0, 60.0},
            {10.0, 110.0}};    // Jump down at x=5
        Piecewise_linear_function expected = {
            {0.0, 80.0},
            {5.0, 50.0}, {5.0, 60.0},
            {10.0, 110.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }

    // Test case 12: Adjacent vertical segments
    {
        Piecewise_linear_function fn1 = {
            {0.0, 60.0},
            {5.0, 80.0}, {5.0, 120.0},
            {6.0, 125.0}, {6.0, 135.0},
            {10.0, 140.0}};
        Piecewise_linear_function fn2 = {
            {0.0, 70.0},
            {5.0, 90.0},
            {6.0, 95.0},
            {10.0, 130.0}};
        Piecewise_linear_function expected = {
            {0.0, 60.0},
            {5.0, 80.0}, {5.0, 90.0},
            {6.0, 95.0},
            {10.0, 130.0}};
        EXPECT_NO_THROW(result = get_lower_envelope(fn1, fn2));
        EXPECT_EQ(result, expected);
    }
}

TEST(piecewise_function, in_range)
{
    // Segment [(-1,-1), (1,1)[
    Segment segment{{-1.0, -1.0}, {1.0, 1.0}};
    // first dot is in the range
    EXPECT_TRUE(segment.x_in_range(-1.0));
    EXPECT_TRUE(segment.y_in_range(-1.0));
    // the last dot is not in the range
    EXPECT_FALSE(segment.y_in_range(1.0));
    EXPECT_FALSE(segment.x_in_range(1.0));
    // in the bounds
    EXPECT_TRUE(segment.x_in_range(0.0));
    EXPECT_TRUE(segment.y_in_range(0.0));
    // out of bounds
    EXPECT_FALSE(segment.x_in_range(-2.0));
    EXPECT_FALSE(segment.x_in_range(2.0));
    EXPECT_FALSE(segment.y_in_range(-2.0));
    EXPECT_FALSE(segment.y_in_range(2.0));
    // vertical segment
    segment = {{-1.0, 1.0}, {-1.0, 3.0}};
    EXPECT_FALSE(segment.x_in_range(-2.0));
    EXPECT_FALSE(segment.x_in_range(-1.0));
    EXPECT_FALSE(segment.x_in_range(2.0));
    EXPECT_FALSE(segment.y_in_range(0.0));
    EXPECT_TRUE(segment.y_in_range(2.0));
    EXPECT_FALSE(segment.y_in_range(3.0));
    EXPECT_FALSE(segment.y_in_range(4.0));
    // horizontal segment
    segment = {{-1.0, 1.0}, {1.0, 1.0}};
    EXPECT_FALSE(segment.x_in_range(-2.0));
    EXPECT_TRUE(segment.x_in_range(-1.0));
    EXPECT_FALSE(segment.x_in_range(1.0));
    EXPECT_FALSE(segment.x_in_range(2.0));
    EXPECT_FALSE(segment.y_in_range(0.0));
    EXPECT_FALSE(segment.y_in_range(1.0));
    EXPECT_FALSE(segment.y_in_range(3.0));
    EXPECT_FALSE(segment.y_in_range(4.0));
}

extern void merge(Piecewise_linear_function &pwf);

TEST(piecewise_function, merge_segments)
{
    Piecewise_linear_function fn = {{-1.0, -1.0}, {0.0, 0.0}, {2.5, 2.5}, {2.6, 2.6}, {4.0, 4.0}};
    Piecewise_linear_function expected = {{-1.0, -1.0}, {4.0, 4.0}};
    EXPECT_NO_THROW(merge(fn));
    EXPECT_EQ(fn, expected);

    fn = {{-1.0, -1.0}, {0.0, 0.0}, {2.5, 2.5}, {2.5, 2.6}, {4.0, 4.0}};
    expected = {{-1.0, -1.0}, {2.5, 2.5}, {2.5, 2.6}, {4.0, 4.0}};
    EXPECT_NO_THROW(merge(fn));
    EXPECT_EQ(fn, expected);

    fn = {{-1.0, -1.0}, {0.0, -1.0}, {0.0, 2.5}, {2.5, 2.5}, {4.0, 2.5}};
    expected = {{-1.0, -1.0}, {0.0, -1.0}, {0.0, 2.5}, {4.0, 2.5}};
    EXPECT_NO_THROW(merge(fn));
    EXPECT_EQ(fn, expected);

    fn = {{-1.0, -1.0}, {0.0, -1.0}, {0.0, 1.0}, {0.0, 2.5}, {2.5, 2.5}, {4.0, 2.5}};
    expected = {{-1.0, -1.0}, {0.0, -1.0}, {0.0, 2.5}, {4.0, 2.5}};
    EXPECT_NO_THROW(merge(fn));
    EXPECT_EQ(fn, expected);
}

TEST(piecewise_function, add_variation)
{
    // init with a flat function (y=100)
    Piecewise_linear_function fn = {{0.0, 100.0}, {100.0, 100.0}};

    // add a variation +50 at x=10
    Segment variation{{10.0, 0.0}, {10.0, 50.0}};
    Piecewise_linear_function expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {100.0, 150.0}};
    Piecewise_linear_function result;
    EXPECT_NO_THROW(result = add_variation(fn, variation));
    EXPECT_EQ(result, expected);

    // add a variation +70 from x=10 to x=20 to the previous result
    variation = {{10.0, 0.0}, {20.0, 70.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {100.0, 220.0}};
    EXPECT_NO_THROW(result = add_variation(result, variation));
    EXPECT_EQ(result, expected);

    // add a variation +80 at x=100
    variation = {{100.0, 0.0}, {100.0, 80.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {100.0, 220.0}, {100.0, 300.0}};
    EXPECT_NO_THROW(result = add_variation(result, variation));
    EXPECT_EQ(result, expected);

    // add a variation -100 at x=0
    variation = {{0.0, 0.0}, {0.0, -100.0}};
    expected = {{0.0, 100.0}, {0.0, 0.0}, {10.0, 0.0}, {10.0, 50.0}, {20.0, 120.0}, {100.0, 120.0}, {100.0, 200.0}};
    EXPECT_NO_THROW(result = add_variation(result, variation));
    EXPECT_EQ(result, expected);
    // add a variation +100 at x=0
    variation = {{0.0, 0.0}, {0.0, 100.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {100.0, 220.0}, {100.0, 300.0}};
    EXPECT_NO_THROW(result = add_variation(result, variation));
    EXPECT_EQ(result, expected);
    // add a variation -50 at x=0
    variation = {{0.0, 0.0}, {0.0, -50.0}};
    expected = {{0.0, 100.0}, {0.0, 50.0}, {10.0, 50.0}, {10.0, 100.0}, {20.0, 170.0}, {100.0, 170.0}, {100.0, 250.0}};
    EXPECT_NO_THROW(result = add_variation(result, variation));
    EXPECT_EQ(result, expected);
    // add a variation +50 at x=100
    variation = {{100.0, 0.0}, {100.0, 50.0}};
    expected = {{0.0, 100.0}, {0.0, 50.0}, {10.0, 50.0}, {10.0, 100.0}, {20.0, 170.0}, {100.0, 170.0}, {100.0, 300.0}};
    EXPECT_NO_THROW(result = add_variation(result, variation));
    EXPECT_EQ(result, expected);
    // Add again +50 at x=0
    variation = {{0.0, 0.0}, {0.0, 50.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {100.0, 220.0}, {100.0, 350.0}};
    EXPECT_NO_THROW(result = add_variation(result, variation));
    EXPECT_EQ(result, expected);
    // Remove 40 at x=80
    variation = {{80.0, 0.0}, {80.0, -40.0}};
    expected = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {80.0, 220.0}, {80.0, 180.0}, {100.0, 180.0}, {100.0, 310.0}};
    EXPECT_NO_THROW(result = add_variation(result, variation));
    EXPECT_EQ(result, expected);
}

TEST(piecewise_function, get_lowest_dot)
{
    Piecewise_linear_function pwf = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 310.0}};
    Dot res;
    // on vertical segment
    EXPECT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 80.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // on horizontal segment
    EXPECT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 10.0));
    EXPECT_TRUE(safecomp::eq(res._x, 00.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on random segment
    EXPECT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 79.0));
    EXPECT_TRUE(safecomp::eq(res._x, 79.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -212.6666667)) << res;
    // on exact dot
    EXPECT_NO_THROW(res = get_lowest_dot(pwf, 15.0, 200.0));
    EXPECT_TRUE(safecomp::eq(res._x, 80.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // out of range
    EXPECT_NO_THROW(res = get_lowest_dot(pwf, 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_lowest_dot(pwf, -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    EXPECT_NO_THROW(res = get_lowest_dot(pwf, -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    EXPECT_NO_THROW(res = get_lowest_dot(pwf, 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
}

TEST(piecewise_function, get_highest_dot)
{
    Piecewise_linear_function pwf = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 310.0}, {20.0, 220.0}, {21.0, 270.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    EXPECT_NO_THROW(res = get_highest_dot(pwf, 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 310.0)) << res;
    // on horizontal segment
    EXPECT_NO_THROW(res = get_highest_dot(pwf, 0.0, 8.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on random segment
    EXPECT_NO_THROW(res = get_highest_dot(pwf, 15.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 15.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 265.)) << res;
    // on exact dot
    EXPECT_NO_THROW(res = get_highest_dot(pwf, 15.0, 80.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 270.0)) << res;
    // out of range
    EXPECT_NO_THROW(res = get_highest_dot(pwf, 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_highest_dot(pwf, -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    EXPECT_NO_THROW(res = get_highest_dot(pwf, -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    EXPECT_NO_THROW(res = get_highest_dot(pwf, 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
}

TEST(piecewise_function, get_first_dot_above)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 110.0}, {10.0, 110.0}, {10.0, 310.0},
        {20.0, 220.0}, {21.0, 270.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 50., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on horizontal segment
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 110, 0.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 5.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 110.0)) << res;
    // on random segment
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 245., 20.0, 22.0));
    EXPECT_TRUE(safecomp::eq(res._x, 20.5)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 245.)) << res;
    // on exact dot
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 310., .0, 120.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 310.0)) << res;
    // out of range
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on first bound
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 0., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on last bound nan since vertical segment
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 300., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 150., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_first_dot_above(pwf, 135., 90.0, 300.0));
    EXPECT_TRUE(safecomp::eq(res._x, 90.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 135.0)) << res;
}
TEST(piecewise_function, get_first_dot_below)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 320.0}, {10.0, 320.0}, {10.0, 310.0},
        {20.0, -180.0}, {21.0, -220.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 150.0}};
    Dot res;
    // on vertical segment
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, 250., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on horizontal segment
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, -220, 21.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // on random segment
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, -200., 20.0, 21.0));
    EXPECT_TRUE(safecomp::eq(res._x, 20.5)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -200.)) << res;
    // on exact dot
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, -220., .0, 120.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // out of range
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, -400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on bounds : nan due to tendancy
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, 10., -100.0, 0.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on bounds : value due to tendancy
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, 120., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.)) << res;
    // on last bound nan since vertical segment
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, 150., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // special case : vertical segment invalid
    EXPECT_NO_THROW(res = get_first_dot_below(pwf, 0., -100.0, 150.0));
    Segment segment{{10.0, 310.0}, {20.0, -180.0}};
    EXPECT_TRUE(safecomp::eq(res._x, segment.get_x(0.0)));
    EXPECT_TRUE(safecomp::eq(res._y, 0.0));
}

TEST(piecewise_function, get_first_dot_of_last_piece_above)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 110.0}, {10.0, 110.0}, {10.0, 310.0},
        {20.0, 220.0}, {21.0, 270.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 50., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 80.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 90.0)) << res;
    // on horizontal segment
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 110, 0.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 5.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 110.0)) << res;
    // on random segment
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 245., 20.0, 22.0));
    EXPECT_TRUE(safecomp::eq(res._x, 20.5)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 245.)) << res;
    // out of bound
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // out of bound
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // in bound but no last piece
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 200., .0, 120.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 0., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 300., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
}
TEST(piecewise_function, get_first_dot_of_last_piece_below)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, -100.0}, {5.0, -110.0}, {10.0, -110.0}, {10.0, -310.0},
        {20.0, -220.0}, {21.0, -270.0}, {80.0, 220.0}, {80.0, -90.0}, {100.0, -180.0}, {100.0, -300.0}};
    Dot res;
    // on vertical segment
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -50., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 80.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -90.0)) << res;
    // on horizontal segment
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -110, 0.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 5.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -110.0)) << res;
    // on random segment
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -245., 20.0, 22.0));
    EXPECT_TRUE(safecomp::eq(res._x, 20.5)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -245.)) << res;
    // out of bound
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // out of bound
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // in bound but no last piece
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -200., .0, 120.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, 0., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -100.0)) << res;
    // on right bound
    EXPECT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -300., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
}

TEST(piecewise_function, get_last_dot_above)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 320.0}, {10.0, 320.0}, {10.0, 310.0},
        {20.0, 325.0}, {21.0, 270.0}, {21.0, 250.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 250., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 250.0)) << res;
    // on horizontal segment
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 320, 0.0, 10.0));
    EXPECT_TRUE(safecomp::eq(res._x, 5.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 320.0)) << res;
    // on random segment
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 317.5, 10.0, 15.0));
    EXPECT_TRUE(safecomp::eq(res._x, 15.)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 317.5)) << res;
    // on exact dot
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 325., .0, 120.0));
    EXPECT_TRUE(safecomp::eq(res._x, 20.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 325.0)) << res;
    // out of range
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bounds
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 0., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    EXPECT_NO_THROW(res = get_last_dot_above(pwf, 300., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
}
TEST(piecewise_function, get_last_dot_below)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 320.0}, {10.0, 320.0}, {10.0, 310.0},
        {20.0, -180.0}, {21.0, -220.0}, {80.0, -220.0}, {80.0, 90.0}, {90.0, 90.0}, {90.0, 80.0}, {100.0, 180.0}, {100.0, 150.0}};
    Dot res;
    // on vertical segment
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, 80., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 90.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 80.0)) << res;
    // on horizontal segment => not 80 since the tendancy is up
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, -200, 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // on random segment
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, 130., 90.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 90.)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 80.)) << res;
    // on exact dot
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, -220., .0, 120.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // out of range
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, -400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, 200., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    EXPECT_NO_THROW(res = get_last_dot_below(pwf, 150., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
}

TEST(piecewise_function, get_last_dot_of_first_piece_below)
{
    // Function with transitions from below to above threshold
    Piecewise_linear_function pwf = {{0.0, 10.0}, {10.0, 50.0}, {20.0, 150.0}, {30.0, 200.0}, {40.0, 100.0}, {50.0, 80.0}, {60.0, 120.0}, {61.0, 120.0}};
    Dot res;

    // Normal case: function starts below threshold and crosses above
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf, 100.0, 0.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 15.0)) << res;  // Intersection point where function crosses y=100
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;

    // Function starts above threshold immediately
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf, 5.0, 0.0, 60.0));
    EXPECT_TRUE(std::isnan(res._x)) << res; // no dot below
    EXPECT_TRUE(std::isnan(res._y)) << res;

    // Function never goes above threshold in given interval
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf, 300.0, 0.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 60.0)) << res;  // Get the last dot
    EXPECT_TRUE(safecomp::eq(res._y, 120.0)) << res;

    // Function crosses exactly on a dot
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf, 50.0, 0.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;  // Exact match on dot
    EXPECT_TRUE(safecomp::eq(res._y, 50.0)) << res;

    // Test with limited interval
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf, 75.0, 35.0, 60.0));
    EXPECT_TRUE(std::isnan(res._x)) << res; // no dot below
    EXPECT_TRUE(std::isnan(res._y)) << res;

    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf, 100.0, 40.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 55.0)) << res;  // Starts at the limit, goes above at 55.
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;

    // Out of bounds - completely before function range
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf, 100.0, -20.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;

    // Out of bounds - completely after function range
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf, 100.0, 70.0, 80.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;

    // Test with vertical segment at threshold
    Piecewise_linear_function pwf_vertical = {{0.0, 50.0}, {10.0, 50.0}, {10.0, 150.0}, {20.0, 200.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf_vertical, 100.0, 0.0, 30.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;  // Vertical segment crosses threshold
    EXPECT_TRUE(safecomp::eq(res._y, 50.0)) << res;

    // Test with horizontal segment at threshold
    Piecewise_linear_function pwf_horizontal = {{0.0, 100.0}, {10.0, 50.0}, {50.0, 50.0}, {50.0, 150.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf_horizontal, 50.0, 0.0, 40.0));
    EXPECT_TRUE(std::isnan(res._x)) << res; // starts above threshold and search is limited to the first piece from x_start, where it never goes below the threshold
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // Test with horizontal segment at threshold
    pwf_horizontal = {{0.0, 30.0}, {10.0, 50.0}, {50.0, 50.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf_horizontal, 51.0, 0.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;  // Function reaches threshold and stays there
    EXPECT_TRUE(safecomp::eq(res._y, 50.0)) << res;

    // Test with horizontal segment below threshold followed by vertical jump
    Piecewise_linear_function pwf_horizontal_vertical = {{0.0, 80.0}, {10.0, 80.0}, {10.0, 120.0}, {20.0, 120.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf_horizontal_vertical, 100.0, 0.0, 30.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;  // Vertical jump crosses threshold
    EXPECT_TRUE(safecomp::eq(res._y, 80.0)) << res;

    // Test with function starting with vertical segment above threshold
    Piecewise_linear_function pwf_start_vertical = {{0.0, 50.0}, {0.0, 150.0}, {10.0, 200.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_below(pwf_start_vertical, 100.0, 0.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;  // Vertical jump crosses threshold
    EXPECT_TRUE(safecomp::eq(res._y, 50.0)) << res; // but first dot is ok
}

TEST(piecewise_function, get_last_dot_of_first_piece_above)
{
    // Function with transitions from above to below threshold (inverted from get_last_dot_of_first_piece_below)
    Piecewise_linear_function pwf = {{0.0, -10.0}, {10.0, -50.0}, {20.0, -150.0}, {30.0, -200.0}, {40.0, -100.0}, {50.0, -80.0}, {60.0, -120.0},  {61.0, -120.0}};
    Dot res;

    // Normal case: function starts above threshold and crosses below
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf, -100.0, 0.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 15.0)) << res;  // Intersection point where function crosses y=-100
    EXPECT_TRUE(safecomp::eq(res._y, -100.0)) << res;

    // Function starts below threshold immediately
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf, -5.0, 0.0, 60.0));
    EXPECT_TRUE(std::isnan(res._x)) << res; // no dot above
    EXPECT_TRUE(std::isnan(res._y)) << res;

    // Function never goes below threshold in given interval
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf, -300.0, 0.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 60.0)) << res;  // Get the last dot
    EXPECT_TRUE(safecomp::eq(res._y, -120.0)) << res;

    // Function crosses exactly on a dot
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf, -50.0, 0.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;  // Exact match on dot
    EXPECT_TRUE(safecomp::eq(res._y, -50.0)) << res;

    // Test with limited interval
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf, -75.0, 35.0, 60.0));
    EXPECT_TRUE(std::isnan(res._x)) << res; // no dot above
    EXPECT_TRUE(std::isnan(res._y)) << res;

    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf, -100.0, 40.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 55.0)) << res;  // Starts at the limit, goes below at 55.
    EXPECT_TRUE(safecomp::eq(res._y, -100.0)) << res;

    // Out of bounds - completely before function range
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf, -100.0, -20.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;

    // Out of bounds - completely after function range
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf, -100.0, 70.0, 80.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;

    // Test with vertical segment at threshold
    Piecewise_linear_function pwf_vertical = {{0.0, -50.0}, {10.0, -50.0}, {10.0, -150.0}, {20.0, -200.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf_vertical, -100.0, 0.0, 30.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;  // Vertical segment crosses threshold
    EXPECT_TRUE(safecomp::eq(res._y, -50.0)) << res;

    // Test with horizontal segment at threshold
    Piecewise_linear_function pwf_horizontal = {{0.0, -100.0}, {10.0, -50.0}, {50.0, -50.0}, {50.0, -150.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf_horizontal, -50.0, 0.0, 40.0));
    EXPECT_TRUE(std::isnan(res._x)) << res; // starts above threshold and search is limited to the first piece from x_start, where it never goes below the threshold
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // Test with horizontal segment at threshold
    pwf_horizontal = {{0.0, -30.0}, {10.0, -50.0}, {50.0, -50.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf_horizontal, -51.0, 0.0, 60.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;  // Function reaches threshold and stays there
    EXPECT_TRUE(safecomp::eq(res._y, -50.0)) << res;

    // Test with horizontal segment above threshold followed by vertical drop
    Piecewise_linear_function pwf_horizontal_vertical = {{0.0, -80.0}, {10.0, -80.0}, {10.0, -120.0}, {20.0, -120.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf_horizontal_vertical, -100.0, 0.0, 30.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;  // Vertical drop crosses threshold
    EXPECT_TRUE(safecomp::eq(res._y, -80.0)) << res;

    // Test with function starting with vertical segment below threshold
    Piecewise_linear_function pwf_start_vertical = {{0.0, -50.0}, {0.0, -150.0}, {10.0, -200.0}};
    EXPECT_NO_THROW(res = get_last_dot_of_first_piece_above(pwf_start_vertical, -100.0, 0.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;  // Vertical jump crosses threshold
    EXPECT_TRUE(safecomp::eq(res._y, -50.0)) << res; // but first dot is ok
}
