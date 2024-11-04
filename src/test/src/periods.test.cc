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

TEST(periods, getInter)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LTimePeriod mylist1, mylist2, interRes, expRes;
    LTimePeriod invalidList =
        { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)))
        , time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)))};

    //invalid list
#ifndef NDEBUG
    ASSERT_THROW(getInter(mylist1, invalidList), std::logic_error);
    ASSERT_THROW(getInter(invalidList, mylist1), std::logic_error);
#endif
    //empty lists
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    //one empty list
    mylist1 = {time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) ) };
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    //list are identical
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
            , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
            , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_EQ(mylist1, mylist2);
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist1);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist1);

#ifndef NDEBUG
    //Invalid list
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
             , time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
             , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_THROW(getInter(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(getInter(mylist2, mylist1), std::logic_error);
#endif

    //Some tests on intersection computation
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
             , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
             , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };

    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist2);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist2);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist2);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist2);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(15)) ) };

    expRes = { time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) )
                       , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };

    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(18)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(18)) ) };

    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
                       , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(18)) ) };

    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);

}

TEST(periods, getUnion)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LTimePeriod mylist1, mylist2, unionRes, expRes;
    LTimePeriod invalidList={
        time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
        time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)))};

    //invalid list
#ifndef NDEBUG
    ASSERT_THROW(getUnion(mylist1, invalidList), std::logic_error);
    ASSERT_THROW(getUnion(invalidList, mylist1), std::logic_error);
#endif
    //empty lists
    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //one empty list
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) ) };
    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, mylist1);
    unionRes = getUnion(mylist2, mylist1);
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
    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //Invalid list
#ifndef NDEBUG
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
              , time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
              , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_THROW(getUnion(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(getUnion(mylist2, mylist1), std::logic_error);
#endif
    //Some tests on union computation
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
              , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
              , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 ={ time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) ) };

    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
              , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, mylist1);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, mylist1);

    mylist1 = {time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
            , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(15)) ) } ;

    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(15)) ) };

    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(18)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(18)) ) };

    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(18)) ) };

    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist1, mylist2);
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
    unionRes = getUnion(mylist2, mylist1, false);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist1, mylist2, false);
    ASSERT_EQ(unionRes, expRes);

}

TEST(periods, getDiff)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LTimePeriod mylist1, mylist2, diffRes, expRes;
    LTimePeriod invalidList = {time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10))),
            time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)))};

    //invalid list
#ifndef NDEBUG
    ASSERT_THROW(getDiff(mylist1, invalidList), std::logic_error);
    ASSERT_THROW(getDiff(invalidList, mylist1), std::logic_error);
#endif
    //empty lists
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    //one empty list
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) ) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, mylist1);
    diffRes = getDiff(mylist2, mylist1);
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
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, LTimePeriod());
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, LTimePeriod());

    //Invalid list
#ifndef NDEBUG
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
             , time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
             , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_THROW(getDiff(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(getDiff(mylist2, mylist1), std::logic_error);
#endif

    //Some tests on difference computation
    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
            , time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) )
            , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
                        , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) ) };

    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, LTimePeriod());

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
            , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) )
                     , time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(12)) )
                     , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, LTimePeriod());


    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
             , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(14)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(11)) , bpt::ptime(d, bpt::hours(15)) ) };
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(11)) ) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    expRes = { time_period( bpt::ptime(d, bpt::hours(12)) , bpt::ptime(d, bpt::hours(13)) )
                       , time_period( bpt::ptime(d, bpt::hours(14)) , bpt::ptime(d, bpt::hours(15)) ) };
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(12)) )
            , time_period( bpt::ptime(d, bpt::hours(13)) , bpt::ptime(d, bpt::hours(18)) ) };
    mylist2 = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(18)) ) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, LTimePeriod());
    expRes = { time_period( bpt::ptime(d, bpt::hours(12)) , bpt::ptime(d, bpt::hours(13)) ) };
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);
}

TEST(periods, shortCuts)
{
    bg::date d = bg::day_clock::local_day();;

    LTimePeriod mylist { time_period( bpt::ptime(d, bpt::hours(10)), bpt::ptime(d, bpt::hours(12)) ) };
    LTimePeriod expRes { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(13)) ) };
    //Union
    ASSERT_EQ( getUnion( time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) , mylist )
                ,  expRes);
    ASSERT_EQ( getUnion( mylist, time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) )
                , expRes );
    ASSERT_EQ( getUnion( time_period( bpt::ptime(d, bpt::hours(10)), bpt::ptime(d, bpt::hours(12)) ), time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) )
                , expRes );

    //Intersection
    expRes = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(12)) ) };
    ASSERT_EQ(getInter( time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) , mylist )
                , expRes );
    ASSERT_EQ(getInter( mylist, time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) )
                , expRes );
    ASSERT_EQ( getInter( time_period( bpt::ptime(d, bpt::hours(10)), bpt::ptime(d, bpt::hours(12)) ), time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) )
                , expRes );

    //Difference
    expRes = { time_period( bpt::ptime(d, bpt::hours(9)) , bpt::ptime(d, bpt::hours(10)) ), time_period( bpt::ptime(d, bpt::hours(12)) , bpt::ptime(d, bpt::hours(13)) ) };
    ASSERT_EQ(getDiff( time_period( bpt::ptime(d, bpt::hours(9)), bpt::ptime(d, bpt::hours(13)) ) , mylist )
                , expRes );
    expRes = { time_period( bpt::ptime(d, bpt::hours(10)) , bpt::ptime(d, bpt::hours(11)) ) };
    ASSERT_EQ(getDiff( mylist, time_period( bpt::ptime(d, bpt::hours(11)), bpt::ptime(d, bpt::hours(12)) ) )
                , expRes );
    ASSERT_EQ(getDiff( time_period( bpt::ptime(d, bpt::hours(10)), bpt::ptime(d, bpt::hours(12)) ), time_period( bpt::ptime(d, bpt::hours(11)), bpt::ptime(d, bpt::hours(12)) ) )
                , expRes );
}
