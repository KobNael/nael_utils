#include <gtest/gtest.h>

#include <nael_utils/period/period_utils.hh>

#include <boost/assign/list_of.hpp>

namespace bg = boost::gregorian;
namespace bpt = boost::posix_time;

TEST(capa_period, basics)
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
#ifndef NDEBUG
    ASSERT_FALSE( can_merge(p1, p2 ) ) << "Periods can not be merged";
    ASSERT_DEATH( merge(p1, p2 ), ".*Assertion `can_merge\\(cp1, cp2\\)' failed.*" );
#endif
}

TEST(capa_period, get_inter)
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
}

TEST(capa_period,get_union)
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
}

TEST(capa_period,get_diff)
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

TEST(capa_period, shortCuts)
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

TEST(capa_period, conversion_capa_period)
{
    bg::date d = bg::day_clock::local_day();

    LCapaPeriod myCapaList { capa_period(10, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)) ) };
    LTimePeriod mylist { time_period( bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) };
    LCapaPeriod expRes { capa_period(10, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(12))) };

    //No union since useless

    //Intersection
    ASSERT_EQ( get_inter( myCapaList, time_period( bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) )
            ,  expRes );
    ASSERT_EQ( get_inter( myCapaList, mylist )
            , expRes );
    ASSERT_EQ( get_inter(
                capa_period(10, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)))
                , time_period(bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(12))) )
            ,  expRes );

}
