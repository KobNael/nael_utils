#include <gtest/gtest.h>

#include <nael_utils/period/period_generation.hh>
#include <nael_utils/log/log.hh>

namespace
{
    bg::date today = bg::day_clock::local_day();
}

namespace
{
    template<class PeriodT>
    void test_basic_lperiod(
        std::list<PeriodT> periods,
        bpt::ptime const &start_horizon, bpt::ptime const &end_horizon,
        bpt::time_duration const &start_period, bpt::time_duration const &end_period)
    {
        EXPECT_EQ(periods.size(), 11ul);
        int counter (0);
        for(PeriodT const &p : periods)
        {
            //First period has been reduced to match start_horizon
            if(0 == counter)
            {
                EXPECT_EQ(p.begin(), start_horizon);
            }
            else
            {
                EXPECT_EQ(p.begin().time_of_day(), start_period);
                EXPECT_EQ(p.begin().date(), today + bg::days(counter));
            }
            //Last period has been reduced to match end_horizon
            if(10 == counter)
            {
                EXPECT_EQ(p.end(), end_horizon);
            }
            else
            {
                EXPECT_EQ(p.end().time_of_day(), end_period);
                EXPECT_EQ(p.end().date(), today + bg::days(counter));
            }
            ++counter;
        }
    }

    template<class PeriodT>
    void test_overlap_lperiod(
        std::list<PeriodT> periods,
        bpt::ptime const &end_horizon,
        bpt::time_duration const &start_period, bpt::time_duration const &end_period)
    {
        EXPECT_EQ(periods.size(), 10ul);
        int counter (0);
        for(PeriodT const &p : periods)
        {
            if(9 == counter)
            {
                EXPECT_TRUE(p.end() <= end_horizon);
            }
            EXPECT_EQ(p.begin().time_of_day(), start_period);
            EXPECT_EQ(p.begin().date(), today + bg::days(counter));
            EXPECT_EQ(p.end().time_of_day(), end_period);
            EXPECT_EQ(p.end().date(), today + bg::days(counter + 1));
            ++counter;
        }
    }
} //namespace

#ifndef NDEBUG
TEST(generate_periods, assertions)
{
    bpt::ptime start_horizon = bpt::ptime(today, bpt::hours(10));
    bpt::ptime end_horizon = bpt::ptime(today + bg::days(10), bpt::hours(20));

    GTEST_FLAG_SET(death_test_style, "threadsafe");
    EXPECT_DEBUG_DEATH(
        generate_periods(end_horizon, start_horizon, bpt::hours(8), bpt::hours(22)),
        ".*Assertion `start_horizon < end_horizon' failed.*" );
    EXPECT_DEBUG_DEATH(
        generate_periods(start_horizon, end_horizon, bpt::hours(-2), bpt::hours(22)),
        ".*Assertion .*start_period && start_period.* failed.*" );
    EXPECT_DEBUG_DEATH(
        generate_periods(start_horizon, end_horizon, bpt::hours(8), bpt::hours(32)),
        ".*Assertion .*end_period && end_period.* failed.*" );

    EXPECT_DEBUG_DEATH(
        generate_periods(bpt::not_a_date_time, end_horizon, bpt::hours(8), bpt::hours(16)),
        ".*Assertion .*start_horizon\\.is_not_a_date_time.* failed.*" );
    EXPECT_DEBUG_DEATH(
        generate_periods(start_horizon, bpt::not_a_date_time, bpt::hours(8), bpt::hours(16)),
        ".*Assertion .*end_horizon\\.is_not_a_date_time.* failed.*" );
    EXPECT_DEBUG_DEATH(
        generate_periods(end_horizon, start_horizon, bpt::hours(8), bpt::hours(16)),
        ".*Assertion .*start_horizon < end_horizon.* failed.*" );
}
#endif

TEST(generate_periods, time_periods)
{
    bpt::ptime start_horizon = bpt::ptime(today, bpt::hours(10));
    bpt::ptime end_horizon = bpt::ptime(today + bg::days(10), bpt::hours(20));

    //Basic period morning / evening
    LTimePeriod generated_periods = generate_periods(start_horizon, end_horizon, bpt::hours(8), bpt::hours(22));
    test_basic_lperiod(generated_periods, start_horizon, end_horizon, bpt::hours(8), bpt::hours(22));

    //Overlaping period evening / morning
    generated_periods = generate_periods(start_horizon, end_horizon, bpt::hours(22), bpt::hours(8));
    test_overlap_lperiod(generated_periods, end_horizon, bpt::hours(22), bpt::hours(8));
}

TEST(generate_periods, capa_periods)
{
    bpt::ptime start_horizon = bpt::ptime(today, bpt::hours(10));
    bpt::ptime end_horizon = bpt::ptime(today + bg::days(10), bpt::hours(20));

    //Basic period morning / evening
    LCapaPeriod generated_periods = generate_periods(start_horizon, end_horizon, bpt::hours(8), bpt::hours(22), 100u);
    test_basic_lperiod(generated_periods, start_horizon, end_horizon, bpt::hours(8), bpt::hours(22));
    for(capa_period const &p : generated_periods)
    {
        EXPECT_EQ(p._capa, 100u);
    }

    //Overlaping period evening / morning
    generated_periods = generate_periods(start_horizon, end_horizon, bpt::hours(22), bpt::hours(8), 100u);
    test_overlap_lperiod(generated_periods, end_horizon, bpt::hours(22), bpt::hours(8));
    for(capa_period const &p : generated_periods)
    {
        EXPECT_EQ(p._capa, 100u);
    }
}

TEST(generate_periods, temporal_mesh)
{
    // 2025 : wednesday 1st january to monday 31 march
    // 20/1 => 20/3 covers 60 days (12 + 28 + 20), 9 weeks (wk 4 to wk 12) and 3 months (jan to mar)
    bpt::ptime start_horizon = bpt::ptime(bg::date(2025, 1, 20), bpt::hours(10));
    bpt::ptime end_horizon = bpt::ptime(bg::date(2025, 3, 20), bpt::hours(20));

    //HORIZON generation
    LTimePeriod generated_periods = generate_periods(start_horizon, end_horizon, TemporalMesh::HORIZON);
    EXPECT_EQ(generated_periods.size(), 1ul);
    EXPECT_EQ(generated_periods.front().begin(), start_horizon);
    EXPECT_EQ(generated_periods.front().end(), end_horizon);

    //DAY generation
    generated_periods = generate_periods(start_horizon, end_horizon, TemporalMesh::DAY);
    EXPECT_EQ(generated_periods.size(), 60ul);
    for(int i(1); i <= 60; ++i)
    {
        auto elt = generated_periods.front();
        generated_periods.pop_front();
        if(1 == i)
        {
            EXPECT_EQ(elt.begin(), start_horizon);
            EXPECT_EQ(elt.end(), bpt::ptime(bg::date(2025, 1, 20), bpt::time_duration(23, 59, 59)));
        }
        else if(60 == i)
        {
            EXPECT_EQ(elt.begin(), bpt::ptime(bg::date(2025, 3, 20), bpt::hours(0)));
            EXPECT_EQ(elt.end(), end_horizon);
        }
        else
        {
            EXPECT_EQ(elt.begin(), bpt::ptime(bg::date(2025, 1, 20) + bg::days(i-1), bpt::hours(0)));
            EXPECT_EQ(elt.end(), bpt::ptime(bg::date(2025, 1, 20) + bg::days(i-1), bpt::time_duration(23, 59, 59)));
        }
    }
    //WEEK generation
    generated_periods = generate_periods(start_horizon, end_horizon, TemporalMesh::WEEK);
    EXPECT_EQ(generated_periods.size(), 9ul);
    for(int i(1); i <= 9; ++i)
    {
        auto elt = generated_periods.front();
        generated_periods.pop_front();
        if(1 == i)
        {
            EXPECT_EQ(elt.begin(), start_horizon);
            EXPECT_EQ(elt.end(), bpt::ptime(bg::date(2025, 1, 26), bpt::time_duration(23, 59, 59)));
        }
        else if(9 == i)
        {
            EXPECT_EQ(elt.begin(), bpt::ptime(bg::date(2025, 3, 17), bpt::hours(0)));
            EXPECT_EQ(elt.end(), end_horizon);
        }
        else
        {
            EXPECT_EQ(elt.begin(), bpt::ptime(bg::date(2025, 1, 27) + bg::days((i-2)*7), bpt::hours(0)));
            EXPECT_EQ(elt.end(), bpt::ptime(bg::date(2025, 2, 2) + bg::days((i-2)*7), bpt::time_duration(23, 59, 59)));
        }
    }
    //MONTH generation
    generated_periods = generate_periods(start_horizon, end_horizon, TemporalMesh::MONTH);
    EXPECT_EQ(generated_periods.size(), 3ul);
    for(int i(1); i <= 3; ++i)
    {
        auto elt = generated_periods.front();
        generated_periods.pop_front();
        if(1 == i)
        {
            EXPECT_EQ(elt.begin(), start_horizon);
            EXPECT_EQ(elt.end(), bpt::ptime(bg::date(2025, 1, 31), bpt::time_duration(23, 59, 59)));
        }
        else if(2 == i)
        {
            EXPECT_EQ(elt.begin(), bpt::ptime(bg::date(2025, 2, 1), bpt::hours(0)));
            EXPECT_EQ(elt.end(), bpt::ptime(bg::date(2025, 2, 28), bpt::time_duration(23, 59, 59)));
        }
        else
        {
            EXPECT_EQ(elt.begin(), bpt::ptime(bg::date(2025, 3, 1), bpt::hours(0)));
            EXPECT_EQ(elt.end(), end_horizon);
        }
    }
    //PERIOD generation
    generated_periods = generate_periods(start_horizon, end_horizon, TemporalMesh::PERIOD);
    EXPECT_EQ(generated_periods.size(), 1ul);
    EXPECT_EQ(generated_periods.front().begin(), start_horizon);
    EXPECT_EQ(generated_periods.front().end(), end_horizon);
}
