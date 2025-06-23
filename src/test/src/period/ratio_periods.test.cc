#include <gtest/gtest.h>

#include <nael_utils/period/period_utils.hh>

#include <boost/assign/list_of.hpp>

namespace bg = boost::gregorian;
namespace bpt = boost::posix_time;

TEST(ratio_periods, basics)
{
    bg::date d = bg::day_clock::local_day();

    //Constructor
    ratio_period p1(1.,  bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)) ),
        p2(1. + 1e-10 , time_period(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) );
    ASSERT_EQ(p1, p2);

    //Stream
    std::ostringstream oss, oss_ref;
    oss << p1;
    oss_ref << "(" << p1._period << "/1)";
    ASSERT_EQ(oss.str(), oss_ref.str());

    //Invalid Merge
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    ASSERT_FALSE( can_merge(p1, p2 ) ) << "Periods can not be merged";
    ASSERT_DEBUG_DEATH( merge(p1, p2 ), ".*Assertion `can_merge\\(rp1, rp2\\)' failed.*" );

    time_period p(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)));
    ratio_period rp1(1., p);
    ASSERT_EQ(rp1._period, p);
    ASSERT_EQ(rp1._ratio, 1.);
    ASSERT_EQ(rp1.begin(), p.begin());
    ASSERT_EQ(rp1.end(), p.end());
}

TEST(ratio_periods, get_union)
{
    bg::date d = bg::day_clock::local_day();

    //Declarations
    LRatioPeriod mylist1, mylist2, unionRes, expRes;

    //empty lists
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //one empty list
    mylist1 = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, mylist1);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, mylist1);

    //list are identity
    mylist1 = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    expRes = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ(mylist1, mylist2);
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

#ifndef NDEBUG
    //Invalid list
    mylist1 = { ratio_period(1., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_THROW(get_union(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(get_union(mylist2, mylist1), std::logic_error);
#endif

    //Some tests on union computation

    //Basic computation
    mylist1 = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(.5, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { ratio_period(2., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { ratio_period(1., bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(12))) };
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //Default value == 1.
    mylist1 = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { ratio_period(2., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { ratio_period(1., bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
                    , ratio_period(2., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
                    , ratio_period(1., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //disjoints slots, with or without merge
    mylist1 = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };
    mylist2 = { ratio_period(1., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    expRes = { ratio_period(1., bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
                    , ratio_period(1., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
                    , ratio_period(1., bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , ratio_period(1., bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14)))
                    , ratio_period(1., bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };

    unionRes = get_union(mylist2, mylist1, false);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist1, mylist2, false);
    ASSERT_EQ(unionRes, expRes);

    expRes = { ratio_period(1., bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(11)))
                    , ratio_period(1., bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(15))) };
    ASSERT_EQ(get_union(mylist1, mylist2, true), expRes);
    ASSERT_EQ(get_union(mylist2, mylist1, true), expRes);

    //null value
    mylist1 = { ratio_period(2., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    mylist2 = { ratio_period(0., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::time_duration(9,30,0)))
            , ratio_period(-2., bpt::ptime(d,bpt::time_duration(9,30,0)) , bpt::ptime(d,bpt::hours(10))) };
    expRes = { ratio_period(0., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::time_duration(9,30,0)))
            , ratio_period(-4., bpt::ptime(d,bpt::time_duration(9,30,0)) , bpt::ptime(d,bpt::hours(10))) };
    unionRes = get_union(mylist2, mylist1, false);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist1, mylist2, false);
    ASSERT_EQ(unionRes, expRes);
}

TEST(ratio_periods, get_inter)
{
    bg::date d = bg::day_clock::local_day();
    //Declarations
    LRatioPeriod myRatioList, interRes, expRes;
    LTimePeriod mylist;

    //empty lists
    interRes = get_inter(myRatioList, mylist);
    ASSERT_EQ(interRes, expRes);
    //Forbidden : do not compile
    //interRes = get_inter(mylist, myRatioList);

    //one empty LTimePeriod
    myRatioList = {ratio_period(1.,  bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)) )};
    interRes = get_inter(myRatioList, mylist);
    ASSERT_EQ(interRes, expRes);

    //list are identical (period wise)
    myRatioList = { ratio_period(.7, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(3.2, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist = { time_period(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , time_period(bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    interRes = get_inter(myRatioList, mylist);
    ASSERT_EQ(interRes, myRatioList);

    //Invalid list
#ifndef NDEBUG
    myRatioList = { ratio_period(1., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(1., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_THROW(get_inter(myRatioList, mylist), std::logic_error);
#endif

    //Some tests on intersection computation

    //time period bigger
    myRatioList = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(2., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , ratio_period(3., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist = { time_period(bpt::ptime(d,bpt::hours(8)) , bpt::ptime(d,bpt::hours(13))) };
    expRes = { ratio_period(1., bpt::ptime(d,bpt::hours( 8)) , bpt::ptime(d,bpt::hours(10)))
                    , ratio_period(2., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
                    , ratio_period(3., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , ratio_period(1., bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };

    interRes = get_inter(myRatioList, mylist);
    ASSERT_EQ(interRes, expRes);

    //time period smaller
    myRatioList = { ratio_period(1., bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , ratio_period(2., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , ratio_period(3., bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist = { time_period(bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { ratio_period(2., bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };

    interRes = get_inter(myRatioList, mylist);
    ASSERT_EQ(interRes, expRes);

}
