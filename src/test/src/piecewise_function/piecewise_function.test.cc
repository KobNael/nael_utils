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

TEST(piecewise_function, get_lowest_dot)
{
    Piecewise_linear_function pwf = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 150.0}, {20.0, 220.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 310.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 80.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // on horizontal segment
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 10.0));
    EXPECT_TRUE(safecomp::eq(res._x, 00.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on random segment
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 0.0, 79.0));
    EXPECT_TRUE(safecomp::eq(res._x, 79.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -212.6666667)) << res;
    // on exact dot
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 15.0, 200.0));
    EXPECT_TRUE(safecomp::eq(res._x, 80.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // out of range
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    ASSERT_NO_THROW(res = get_lowest_dot(pwf, 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
}

TEST(piecewise_function, get_highest_dot)
{
    Piecewise_linear_function pwf = {{0.0, 100.0}, {10.0, 100.0}, {10.0, 310.0}, {20.0, 220.0}, {21.0, 270.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 310.0)) << res;
    // on horizontal segment
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 0.0, 8.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on random segment
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 15.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 15.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 265.)) << res;
    // on exact dot
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 15.0, 80.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 270.0)) << res;
    // out of range
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_highest_dot(pwf, -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    ASSERT_NO_THROW(res = get_highest_dot(pwf, -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    ASSERT_NO_THROW(res = get_highest_dot(pwf, 100.0, 300.0));
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
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 50., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on horizontal segment
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 110, 0.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 5.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 110.0)) << res;
    // on random segment
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 245., 20.0, 22.0));
    EXPECT_TRUE(safecomp::eq(res._x, 20.5)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 245.)) << res;
    // on exact dot
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 310., .0, 120.0));
    EXPECT_TRUE(safecomp::eq(res._x, 10.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 310.0)) << res;
    // out of range
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on first bound
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 0., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on last bound nan since vertical segment
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 300., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 150., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_first_dot_above(pwf, 135., 90.0, 300.0));
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
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 250., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on horizontal segment
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, -220, 21.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // on random segment
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, -200., 20.0, 21.0));
    EXPECT_TRUE(safecomp::eq(res._x, 20.5)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -200.)) << res;
    // on exact dot
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, -220., .0, 120.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // out of range
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, -400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on bounds : nan due to tendancy
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 10., -100.0, 0.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on bounds : value due to tendancy
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 120., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.)) << res;
    // on last bound nan since vertical segment
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 150., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // special case : vertical segment invalid
    ASSERT_NO_THROW(res = get_first_dot_below(pwf, 0., -100.0, 150.0));
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
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 50., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 80.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 90.0)) << res;
    // on horizontal segment
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 110, 0.0, 20.0));
    EXPECT_TRUE(safecomp::eq(res._x, 5.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 110.0)) << res;
    // on random segment
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 245., 20.0, 22.0));
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
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // in bound but no last piece
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 200., .0, 120.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 0., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_above(pwf, 300., 100.0, 300.0));
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
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -50., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 80.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -90.0)) << res;
    // on horizontal segment
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -110, 0.0, 20.0));
    ASSERT_TRUE(safecomp::eq(res._x, 5.0)) << res;
    ASSERT_TRUE(safecomp::eq(res._y, -110.0)) << res;
    // on random segment
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -245., 20.0, 22.0));
    ASSERT_TRUE(safecomp::eq(res._x, 20.5)) << res;
    ASSERT_TRUE(safecomp::eq(res._y, -245.)) << res;
    // out of bound
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -10., 200.0, 300.0));
    ASSERT_TRUE(std::isnan(res._x)) << res;
    ASSERT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -30., -200.0, -10.0));
    ASSERT_TRUE(std::isnan(res._x)) << res;
    ASSERT_TRUE(std::isnan(res._y)) << res;
    // out of bound
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -400., -200.0, 400.0));
    ASSERT_TRUE(std::isnan(res._x)) << res;
    ASSERT_TRUE(std::isnan(res._y)) << res;
    // in bound but no last piece
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -200., .0, 120.0));
    ASSERT_TRUE(std::isnan(res._x)) << res;
    ASSERT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, 0., -100.0, 0.0));
    ASSERT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    ASSERT_TRUE(safecomp::eq(res._y, -100.0)) << res;
    // on right bound
    ASSERT_NO_THROW(res = get_first_dot_of_last_piece_below(pwf, -300., 100.0, 300.0));
    ASSERT_TRUE(std::isnan(res._x)) << res;
    ASSERT_TRUE(std::isnan(res._y)) << res;
}

TEST(piecewise_function, get_last_dot_above)
{
    Piecewise_linear_function pwf =
        {{0.0, 0.0}, {0.0, 100.0}, {5.0, 320.0}, {10.0, 320.0}, {10.0, 310.0},
        {20.0, 325.0}, {21.0, 270.0}, {21.0, 250.0}, {80.0, -220.0}, {80.0, 90.0}, {100.0, 180.0}, {100.0, 300.0}};
    Dot res;
    // on vertical segment
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 250., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 250.0)) << res;
    // on horizontal segment
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 320, 0.0, 10.0));
    EXPECT_TRUE(safecomp::eq(res._x, 5.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 320.0)) << res;
    // on random segment
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 317.5, 10.0, 15.0));
    EXPECT_TRUE(safecomp::eq(res._x, 15.)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 317.5)) << res;
    // on exact dot
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 325., .0, 120.0));
    EXPECT_TRUE(safecomp::eq(res._x, 20.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 325.0)) << res;
    // out of range
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bounds
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 0., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    ASSERT_NO_THROW(res = get_last_dot_above(pwf, 300., 100.0, 300.0));
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
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 80., 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 90.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 80.0)) << res;
    // on horizontal segment => not 80 since the tendancy is up
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, -200, 0.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // on random segment
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 130., 90.0, 100.0));
    EXPECT_TRUE(safecomp::eq(res._x, 90.)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 80.)) << res;
    // on exact dot
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, -220., .0, 120.0));
    EXPECT_TRUE(safecomp::eq(res._x, 21.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, -220.0)) << res;
    // out of range
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 10., 200.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 30., -200.0, -10.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, -400., -200.0, 400.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
    // on left bound
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 200., -100.0, 0.0));
    EXPECT_TRUE(safecomp::eq(res._x, 0.0)) << res;
    EXPECT_TRUE(safecomp::eq(res._y, 100.0)) << res;
    // on right bound
    ASSERT_NO_THROW(res = get_last_dot_below(pwf, 150., 100.0, 300.0));
    EXPECT_TRUE(std::isnan(res._x)) << res;
    EXPECT_TRUE(std::isnan(res._y)) << res;
}
