#include <gtest/gtest.h>

#include <nael_utils/period/period_utils.hh>

namespace bg=boost::gregorian;
namespace bpt=boost::posix_time;

TEST(ratio_periods, total_duration)
{
    bg::date d = bg::day_clock::local_day();

    LRatioPeriod mylist;
    EXPECT_EQ( get_total_duration(mylist), bpt::seconds(0) );

    mylist = { ratio_period(1., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::hours(1) );

    mylist = { ratio_period(1., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        ratio_period(1., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::hours(2) );

    mylist = { ratio_period(1., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        ratio_period(1., bpt::ptime(d, bpt::time_duration(10, 20, 30)) , bpt::ptime(d, bpt::time_duration(10, 21, 32))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::time_duration(1, 1, 2) );
}

TEST(ratio_periods, relative_duration)
{
    bg::date d = bg::day_clock::local_day();

    LRatioPeriod mylist;
    EXPECT_EQ( get_relative_duration(mylist), bpt::seconds(0) );

    mylist = { ratio_period(1., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))) };
    EXPECT_EQ( get_relative_duration(mylist), bpt::hours(1) );

    mylist = { ratio_period(1., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        ratio_period(1., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))) };
    EXPECT_EQ( get_relative_duration(mylist), bpt::hours(2) );

    mylist = { ratio_period(1., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        ratio_period(1., bpt::ptime(d, bpt::time_duration(10, 20, 30)) , bpt::ptime(d, bpt::time_duration(10, 21, 32))) };
    EXPECT_EQ( get_relative_duration(mylist), bpt::time_duration(1, 1, 2) );

    mylist = { ratio_period(0., bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        ratio_period(0., bpt::ptime(d, bpt::time_duration(10, 20, 30)) , bpt::ptime(d, bpt::time_duration(10, 21, 32))) };
    EXPECT_EQ( get_relative_duration(mylist), bpt::seconds(0) );

    mylist = { ratio_period(0.5, bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        ratio_period(1., bpt::ptime(d, bpt::time_duration(10, 20, 30)) , bpt::ptime(d, bpt::time_duration(10, 21, 32))) };
    EXPECT_EQ( get_relative_duration(mylist), bpt::time_duration(0, 31, 2) );
}

TEST(capa_periods, total_duration)
{
    bg::date d = bg::day_clock::local_day();

    LCapaPeriod mylist;
    EXPECT_EQ( get_total_duration(mylist), bpt::seconds(0) );

    mylist = { capa_period(1, bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::hours(1) );

    mylist = { capa_period(1, bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        capa_period(1, bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::hours(2) );

    mylist = { capa_period(1, bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        capa_period(1, bpt::ptime(d, bpt::time_duration(10, 20, 30)) , bpt::ptime(d, bpt::time_duration(10, 21, 32))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::time_duration(1, 1, 2) );
}

TEST(periods, total_duration)
{
    bg::date d = bg::day_clock::local_day();

    LTimePeriod mylist;
    EXPECT_EQ( get_total_duration(mylist), bpt::seconds(0) );

    mylist = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::hours(1) );

    mylist = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::hours(2) );

    mylist = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        time_period( bpt::ptime(d, bpt::time_duration(10, 20, 30)) , bpt::ptime(d, bpt::time_duration(10, 21, 32))) };
    EXPECT_EQ( get_total_duration(mylist), bpt::time_duration(1, 1, 2) );
}

TEST(periods, compute_relative_duration)
{
    //1 hour with a 100% rate => is equivalent to 60' of theoretical production
    EXPECT_EQ(bpt::minutes(60), compute_relative_duration( bpt::hours(1), 1.));
    //1 hour with a 200% rate => is equivalent to 120' of theoretical production
    EXPECT_EQ(bpt::minutes(120), compute_relative_duration( bpt::hours(1), 2.));
    //1 hour with a 50% rate => is equivalent to 30' of theoretical production
    EXPECT_EQ(bpt::minutes(30), compute_relative_duration( bpt::hours(1), .5));
    //1 hour with a 30% rate => is equivalent to 18' of theoretical production
    EXPECT_EQ(bpt::minutes(18), compute_relative_duration( bpt::hours(1), .3));
    //1 hour with a 33% rate => is equivalent to 19'48" of theoretical production
    EXPECT_EQ(bpt::time_duration(0,19,48), compute_relative_duration( bpt::hours(1), .33));
}

TEST(periods, compute_theoretical_duration)
{
    //1 hour of theoretical production with a 100% rate => can be made in 60'
    EXPECT_EQ(bpt::hours(1), compute_theoretical_duration( bpt::minutes(60), 1.));
    //2 hours of theoretical production with a 200% rate => can be made 60'
    EXPECT_EQ(bpt::hours(1), compute_theoretical_duration( bpt::minutes(120), 2.));
    //30' of theoretical production with a 50% rate => can be made in 60'
    EXPECT_EQ(bpt::hours(1), compute_theoretical_duration( bpt::minutes(30), .5));
    //18' of theoretical production with a 30% rate => can be made in 60'
    EXPECT_EQ(bpt::hours(1), compute_theoretical_duration( bpt::minutes(18), .3));
    //19'48" of theoretical production with a 33% rate => can be made in 60'
    EXPECT_EQ(bpt::hours(1), compute_theoretical_duration( bpt::time_duration(0,19,48), .33));
}

TEST(date_time, is_same)
{
    bg::date d = bg::day_clock::local_day();

    bpt::ptime lhs(d, bpt::time_duration(1, 2, 3, 4));
    bpt::ptime rhs = lhs;
    EXPECT_TRUE( is_same(lhs, rhs) );

    rhs = bpt::ptime(d, bpt::time_duration(1, 2, 3, 1000));
    EXPECT_TRUE( is_same(lhs, rhs) ) << lhs << " == " << rhs << " with a default tolerance";

    rhs = bpt::ptime(d, bpt::time_duration(1, 2, 3, 30000));
    EXPECT_FALSE( is_same(lhs, rhs) ) << lhs << " != " << rhs << " with a default tolerance";
    EXPECT_TRUE( is_same(lhs, rhs, boost::posix_time::milliseconds(30)) ) << lhs << " == " << rhs << " with a tolerance of " << boost::posix_time::milliseconds(30);
}

TEST(time_duration, is_same)
{
    bpt::time_duration lhs(1, 2, 3, 4);
    bpt::time_duration rhs = lhs;
    EXPECT_TRUE( is_same(lhs, rhs) );

    rhs = bpt::time_duration(1, 2, 3, 1000);
    EXPECT_TRUE( is_same(lhs, rhs) ) << lhs << " == " << rhs << " with a default tolerance";

    rhs = bpt::time_duration(1, 2, 3, 30000);
    EXPECT_FALSE( is_same(lhs, rhs) ) << lhs << " != " << rhs << " with a default tolerance";
    EXPECT_TRUE( is_same(lhs, rhs, boost::posix_time::milliseconds(30)) ) << lhs << " == " << rhs << " with a tolerance of " << boost::posix_time::milliseconds(30);
}
