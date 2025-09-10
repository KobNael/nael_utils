#include <gtest/gtest.h>

#include <nael_utils/period/period_utils.hh>

#include <boost/assign/list_of.hpp>

namespace bg = boost::gregorian;
namespace bpt = boost::posix_time;

TEST(capa_periods, basics)
{
    bg::date d = bg::day_clock::local_day();

    //Constructor
    capa_period p1(1l,  bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)) ),
        p2(1, time_period(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) );
    ASSERT_EQ(p1, p2);

    //Stream
    std::ostringstream oss, oss_ref;
    oss << p1;
    oss_ref << "(" << p1._period << "/1)";
    ASSERT_EQ(oss.str(), oss_ref.str());

    //Invalid Merge
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    ASSERT_FALSE( can_merge(p1, p2 ) ) << "Periods can not be merged";
    ASSERT_DEBUG_DEATH( merge(p1, p2 ), ".*Assertion `can_merge\\(cp1, cp2\\)' failed.*" );

    time_period p(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)));
    capa_period cp1(1l, p);
    ASSERT_EQ(cp1._period, p);
    ASSERT_EQ(cp1._capa, 1l);
    ASSERT_EQ(cp1.begin(), p.begin());
    ASSERT_EQ(cp1.end(), p.end());
}

TEST(capa_periods, get_inter)
{
    bg::date d = bg::day_clock::local_day();
    //Declarations
    LCapaPeriod mylist1, mylist2, interRes, expRes;

    //empty lists
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    //one empty list
    mylist1 = {capa_period(1,  bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)) )};
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    //list are identical
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ(mylist1, mylist2);
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist1);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist1);

    //Invalid list
#ifndef NDEBUG
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_THROW(get_inter(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(get_inter(mylist2, mylist1), std::logic_error);
#endif
    //Some tests on intersection computation
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };

    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist2);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist2);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) } ;
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist2);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist2);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(15))) };

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };

    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(18))) };

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
                        , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };

    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);

    //null sum
    mylist1 = { capa_period(2, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    mylist2 = { capa_period(0, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::time_duration(9,30,0)))
            , capa_period(-2, bpt::ptime(d,bpt::time_duration(9,30,0)) , bpt::ptime(d,bpt::hours(10))) };

    expRes = { capa_period(-2, bpt::ptime(d,bpt::time_duration(9,30,0)) , bpt::ptime(d,bpt::hours(10))) };
    interRes = get_inter(mylist2, mylist1, false);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist1, mylist2, false);
    ASSERT_EQ(interRes, expRes);
}

TEST(capa_periods, get_union)
{
    bg::date d = bg::day_clock::local_day();

    //Declarations
    LCapaPeriod mylist1, mylist2, unionRes, expRes;

    //empty lists
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //one empty list
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, mylist1);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, mylist1);

    //list are identical
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    expRes = { capa_period(2, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ(mylist1, mylist2);
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

#ifndef NDEBUG
    //Invalid list
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_THROW(get_union(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(get_union(mylist2, mylist1), std::logic_error);
#endif
    //Some tests on union computation
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };

    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(15))) };

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14)))
                     , capa_period(1, bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };

    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(18))) };
    expRes = { capa_period(2, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };

    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);

    //disjoints slots, no merge
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
            , capa_period(1, bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };
    unionRes = get_union(mylist2, mylist1, false);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist1, mylist2, false);
    ASSERT_EQ(unionRes, expRes);

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(15))) };
    ASSERT_EQ(get_union(mylist1, mylist2, true), expRes);
    ASSERT_EQ(get_union(mylist2, mylist1, true), expRes);

    //null sum
    mylist1 = { capa_period(2, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    mylist2 = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::time_duration(9,30,0)))
            , capa_period(-2, bpt::ptime(d,bpt::time_duration(9,30,0)) , bpt::ptime(d,bpt::hours(10))) };

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::time_duration(9,30,0))) };
    unionRes = get_union(mylist2, mylist1, false);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist1, mylist2, false);
    ASSERT_EQ(unionRes, expRes);

}

TEST(capa_periods, get_diff)
{
    bg::date d = bg::day_clock::local_day();

    //Declarations
    LCapaPeriod mylist1, mylist2, diffRes, expRes;

    //empty lists
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    //one empty list
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, mylist1);

    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    //list are identical
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ(mylist1, mylist2);
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, LCapaPeriod());
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, LCapaPeriod());

    //Invalid list
#ifndef NDEBUG
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_THROW(get_diff(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(get_diff(mylist2, mylist1), std::logic_error);
#endif
    //Some tests on difference computation
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };

    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);

    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);

    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(15))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(18))) };
    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { capa_period(2, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(4, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(18))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(3, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);

    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(-3, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);
}

TEST(capa_periods, shortCuts)
{
    bg::date d = bg::day_clock::local_day();

    LCapaPeriod mylist { capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))) };
    LCapaPeriod expRes;

    //Union
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(2, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };
    ASSERT_EQ( get_union( capa_period(1 , bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))), mylist )
                ,  expRes );
    ASSERT_EQ( get_union( mylist, capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))) )
                , expRes );
    ASSERT_EQ( get_union( capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))),
                capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))) )
                , expRes );

    //Intersection
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ( get_inter( capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))), mylist )
                , expRes );

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ( get_inter( mylist, capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))) )
                , expRes );
    ASSERT_EQ( get_inter(
            capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))),
            capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))) )
            , expRes );

    //Difference
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1,  bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };
    ASSERT_EQ( get_diff( capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))), mylist )
                , expRes );
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)) ) };
    ASSERT_EQ( get_diff( mylist, capa_period(1, bpt::ptime(d,bpt::hours(11)), bpt::ptime(d,bpt::hours(12))) )
                , expRes );
    ASSERT_EQ( get_diff(
        capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))),
        capa_period(1, bpt::ptime(d,bpt::hours(11)), bpt::ptime(d,bpt::hours(12))))
        , expRes) ;

}

TEST(capa_periods, conversion)
{
    bg::date d = bg::day_clock::local_day();

    //----------------
    // Period => Capa
    //----------------


    LCapaPeriod myCapaList { capa_period(10, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)) ) };
    LTimePeriod mylist {
        time_period( bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(11)) )
        , time_period( bpt::ptime(d,bpt::hours(13)), bpt::ptime(d,bpt::hours(14)) ) };
    LCapaPeriod expCapaRes {
        capa_period(5, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(11)) )
        , capa_period(5, bpt::ptime(d,bpt::hours(13)), bpt::ptime(d,bpt::hours(14)) )
    };
    ASSERT_EQ(expCapaRes, convert(mylist, 5L));

    expCapaRes = { capa_period(10, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(12))) };
    //No union since it make non sense

    //Intersection
    ASSERT_EQ( get_inter( myCapaList, time_period( bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) )
            ,  expCapaRes );
    ASSERT_EQ( get_inter(
                capa_period(10, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)))
                , time_period(bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(12))) )
            ,  expCapaRes );
    expCapaRes = { capa_period(10, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    ASSERT_EQ( get_inter( myCapaList, mylist )
            , expCapaRes );

    //Difference
    ASSERT_EQ( get_diff( myCapaList, time_period( bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) )
            , LCapaPeriod() );
    expCapaRes = { capa_period(10, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ( get_diff( myCapaList, mylist )
            , expCapaRes );
    ASSERT_EQ( get_diff(
                capa_period(10, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)))
                , time_period(bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(12))) )
            , LCapaPeriod() );

    //----------------
    // Capa => Period
    //----------------
    mylist = { time_period(bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)) ) };
    myCapaList = {
        capa_period(10, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(11)) )
        , capa_period(10, bpt::ptime(d,bpt::hours(13)), bpt::ptime(d,bpt::hours(14)) ) };
    LTimePeriod expRes { time_period(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(13))) };
    //Union
    ASSERT_EQ( get_union( mylist, capa_period(10, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) )
            ,  expRes );
    expRes = { time_period(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ( get_union(
                time_period(bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)))
                , capa_period(10, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(12))) )
            ,  expRes );
    expRes = {
        time_period(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
        ,time_period(bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    ASSERT_EQ( get_union( mylist, myCapaList )
            , expRes );

    //Intersection
    expRes = { time_period(bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ( get_inter( mylist, capa_period(10, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) )
            ,  expRes );
    ASSERT_EQ( get_inter(
                time_period(bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)))
                , capa_period(10, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(12))) )
            ,  expRes );
    expRes = { time_period(bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    ASSERT_EQ( get_inter( mylist, myCapaList )
            , expRes );

    //Difference
    ASSERT_EQ( get_diff( mylist, capa_period(10, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) )
            , LTimePeriod() );
    expRes = { time_period(bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ( get_diff( mylist, myCapaList )
            , expRes );
    ASSERT_EQ( get_diff(
                time_period(bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)))
                , capa_period(10, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(12))) )
            , LTimePeriod() );

}

TEST(capa_periods, get_eligible_periods)
{
    bg::date d = bg::day_clock::local_day();
    LCapaPeriod mylist = {
          capa_period(10, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
        , capa_period(20, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
        , capa_period(30, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };

    LTimePeriod expRes = {
        time_period(bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
        , time_period(bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
        , time_period(bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };

    EXPECT_EQ( get_eligible_periods(mylist, 1), expRes );
    EXPECT_EQ( get_eligible_periods(mylist, 10), expRes );

    expRes = {
        time_period(bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
        , time_period(bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    EXPECT_EQ( get_eligible_periods(mylist, 11), expRes );
    EXPECT_EQ( get_eligible_periods(mylist, 19), expRes );
    EXPECT_EQ( get_eligible_periods(mylist, 20), expRes );

    expRes = { time_period(bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    EXPECT_EQ( get_eligible_periods(mylist, 21), expRes );
    EXPECT_EQ( get_eligible_periods(mylist, 29), expRes );
    EXPECT_EQ( get_eligible_periods(mylist, 30), expRes );

    EXPECT_EQ( get_eligible_periods(mylist, 31), LTimePeriod() );
    EXPECT_EQ( get_eligible_periods(mylist, 99), LTimePeriod() );
}