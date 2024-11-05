#include <gtest/gtest.h>

#include <utils/period/period_utils.hh>

#include <boost/assign/list_of.hpp>

namespace bg=boost::gregorian;
namespace bpt=boost::posix_time;

TEST(periods, order)
{
    bg::date d = bg::day_clock::local_day();;
    ASSERT_TRUE(bpt::ptime(d, bpt::hours(9)) < bpt::not_a_date_time);
    ASSERT_TRUE(bpt::not_a_date_time > bpt::ptime(d, bpt::hours(9)) );
}

TEST(periods, get_inter)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LTimePeriod mylist1, mylist2, interRes, expRes;
    LTimePeriod invalidList =
        { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)))
        , time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)))};

    //invalid list
#ifndef NDEBUG
    ASSERT_THROW(get_inter(mylist1, invalidList), std::logic_error);
    ASSERT_THROW(get_inter(invalidList, mylist1), std::logic_error);
#endif
    //empty lists
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    //one empty list
    mylist1 = {time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) ) };
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    //list are identical
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
            , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
            , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_EQ(mylist1, mylist2);
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist1);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist1);

#ifndef NDEBUG
    //Invalid list
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
             , time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
             , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_THROW(get_inter(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(get_inter(mylist2, mylist1), std::logic_error);
#endif

    //Some tests on intersection computation
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
             , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
             , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };

    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist2);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist2);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist2);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist2);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(15)) ) };

    expRes = { time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) )
                       , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };

    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(18)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(18)) ) };

    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
                       , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(18)) ) };

    interRes = get_inter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);
    interRes = get_inter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);

}

TEST(periods, get_union)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LTimePeriod mylist1, mylist2, unionRes, expRes;
    LTimePeriod invalidList={
        time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)))};

    //invalid list
#ifndef NDEBUG
    ASSERT_THROW(get_union(mylist1, invalidList), std::logic_error);
    ASSERT_THROW(get_union(invalidList, mylist1), std::logic_error);
#endif
    //empty lists
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //one empty list
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) ) };
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, mylist1);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, mylist1);

    //list are identical
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
              , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
              , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) )};
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
              , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
              , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) )};
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_EQ(mylist1, mylist2);
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //Invalid list
#ifndef NDEBUG
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
              , time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
              , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_THROW(get_union(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(get_union(mylist2, mylist1), std::logic_error);
#endif
    //Some tests on union computation
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
              , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
              , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 ={ time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) ) };

    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
              , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, mylist1);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, mylist1);

    mylist1 = {time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
            , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(15)) ) } ;

    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(15)) ) };

    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(18)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(18)) ) };

    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(18)) ) };

    unionRes = get_union(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);

    //disjoints slots, no merge
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
              , time_period( bpt::ptime(d, bpt::hours(12)) , bpt::ptime(d, bpt::hours(13)) )
              , time_period( bpt::ptime(d, bpt::hours(14)) , bpt::ptime(d, bpt::hours(15)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
              , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };

    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
                        , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
                        , time_period( bpt::ptime(d, bpt::hours(12)) , bpt::ptime(d, bpt::hours(13)) )
                        , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) )
                        , time_period( bpt::ptime(d, bpt::hours(14)) , bpt::ptime(d, bpt::hours(15)) ) };
    unionRes = get_union(mylist2, mylist1, false);
    ASSERT_EQ(unionRes, expRes);
    unionRes = get_union(mylist1, mylist2, false);
    ASSERT_EQ(unionRes, expRes);

}

TEST(periods, get_diff)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LTimePeriod mylist1, mylist2, diffRes, expRes;
    LTimePeriod invalidList = {time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
            time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)))};

    //invalid list
#ifndef NDEBUG
    ASSERT_THROW(get_diff(mylist1, invalidList), std::logic_error);
    ASSERT_THROW(get_diff(invalidList, mylist1), std::logic_error);
#endif
    //empty lists
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    //one empty list
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) ) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, mylist1);
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, mylist2);

    //list are identical
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
            , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
            , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
            , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
            , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_EQ(mylist1, mylist2);
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, LTimePeriod());
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, LTimePeriod());

    //Invalid list
#ifndef NDEBUG
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
             , time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
             , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_THROW(get_diff(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(get_diff(mylist2, mylist1), std::logic_error);
#endif

    //Some tests on difference computation
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
            , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
            , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
                        , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };

    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, LTimePeriod());

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
            , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
                     , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) )
                     , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, LTimePeriod());


    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(15)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(11)) ) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    expRes = { time_period( bpt::ptime(d, bpt::hours(12)) , bpt::ptime(d, bpt::hours(13)) )
                       , time_period( bpt::ptime(d, bpt::hours(14)) , bpt::ptime(d, bpt::hours(15)) ) };
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
            , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(18)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(18)) ) };
    diffRes = get_diff(mylist1, mylist2);
    ASSERT_EQ(diffRes, LTimePeriod());
    expRes = { time_period( bpt::ptime(d, bpt::hours(12)) , bpt::ptime(d, bpt::hours(13)) ) };
    diffRes = get_diff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);
}

TEST(periods, shortCuts)
{
    bg::date d = bg::day_clock::local_day();;

    LTimePeriod mylist { time_period( bpt::ptime(d, bpt::hours(10)), bpt::ptime(d, bpt::hours(12)) ) };
    LTimePeriod expRes { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(13)) ) };
    //Union
    ASSERT_EQ( get_union( time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) , mylist )
                ,  expRes);
    ASSERT_EQ( get_union( mylist, time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) )
                , expRes );
    ASSERT_EQ( get_union( time_period( bpt::ptime(d, bpt::hours(10)), bpt::ptime(d, bpt::hours(12)) ), time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) )
                , expRes );

    //Intersection
    expRes = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_EQ(get_inter( time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) , mylist )
                , expRes );
    ASSERT_EQ(get_inter( mylist, time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) )
                , expRes );
    ASSERT_EQ( get_inter( time_period( bpt::ptime(d, bpt::hours(10)), bpt::ptime(d, bpt::hours(12)) ), time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) )
                , expRes );

    //Difference
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) ), time_period( bpt::ptime(d, bpt::hours(12)) , bpt::ptime(d, bpt::hours(13)) ) };
    ASSERT_EQ(get_diff( time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) , mylist )
                , expRes );
    expRes = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    ASSERT_EQ(get_diff( mylist, time_period( bpt::ptime(d, bpt::hours(11)), bpt::ptime(d, bpt::hours(12)) ) )
                , expRes );
    ASSERT_EQ(get_diff( time_period( bpt::ptime(d, bpt::hours(10)), bpt::ptime(d, bpt::hours(12)) ), time_period( bpt::ptime(d, bpt::hours(11)), bpt::ptime(d, bpt::hours(12)) ) )
                , expRes );
}
