#include <gtest/gtest.h>

#include <utils/period/period_utils.hh>

#include <boost/assign/list_of.hpp>

namespace bg = boost::gregorian;
namespace bpt = boost::posix_time;

TEST(capa_period, getInter)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LCapaPeriod mylist1, mylist2, interRes, expRes;

    //empty lists
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    //one empty list
    mylist1 = {capa_period(1,  bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)) )};
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    //list are identical
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ(mylist1, mylist2);
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist1);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist1);

    //Invalid list
#ifndef NDEBUG
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_THROW(getInter(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(getInter(mylist2, mylist1), std::logic_error);
#endif
    //Some tests on intersection computation
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };

    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist2);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist2);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) } ;
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, mylist2);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, mylist2);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(15))) };

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };

    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(18))) };

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
                        , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };

    interRes = getInter(mylist2, mylist1);
    ASSERT_EQ(interRes, expRes);
    interRes = getInter(mylist1, mylist2);
    ASSERT_EQ(interRes, expRes);
}

TEST(capa_period,getUnion)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LCapaPeriod mylist1, mylist2, unionRes, expRes;

    //empty lists
    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    //one empty list
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, mylist1);
    unionRes = getUnion(mylist2, mylist1);
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
    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

#ifndef NDEBUG
    //Invalid list
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_THROW(getUnion(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(getUnion(mylist2, mylist1), std::logic_error);
#endif
    //Some tests on union computation
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };

    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(15))) };

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14)))
                     , capa_period(1, bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };

    unionRes = getUnion(mylist1, mylist2);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(18))) };
    expRes = { capa_period(2, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(2, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };

    unionRes = getUnion(mylist2, mylist1);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist1, mylist2);
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
    unionRes = getUnion(mylist2, mylist1, false);
    ASSERT_EQ(unionRes, expRes);
    unionRes = getUnion(mylist1, mylist2, false);
    ASSERT_EQ(unionRes, expRes);

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(15))) };
    ASSERT_EQ(getUnion(mylist1, mylist2, true), expRes);
    ASSERT_EQ(getUnion(mylist2, mylist1, true), expRes);
}

TEST(capa_period,getDiff)
{
    bg::date d = bg::day_clock::local_day();;

    //Declarations
    LCapaPeriod mylist1, mylist2, diffRes, expRes;

    //empty lists
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    //one empty list
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, mylist1);

    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10))) };
    diffRes = getDiff(mylist2, mylist1);
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
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, LCapaPeriod());
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, LCapaPeriod());

    //Invalid list
#ifndef NDEBUG
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_THROW(getDiff(mylist1, mylist2), std::logic_error);
    ASSERT_THROW(getDiff(mylist2, mylist1), std::logic_error);
#endif
    //Some tests on difference computation
    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)))
            , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };

    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);

    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12))) };
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);

    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(14))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(11)) , bpt::ptime(d,bpt::hours(15))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(11)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(14)) , bpt::ptime(d,bpt::hours(15))) };
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(18))) };
    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);

    mylist1 = { capa_period(2, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(4, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    mylist2 = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(18))) };
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(-1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(3, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    diffRes = getDiff(mylist1, mylist2);
    ASSERT_EQ(diffRes, expRes);

    expRes = { capa_period(-1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(12)))
                    , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13)))
                    , capa_period(-3, bpt::ptime(d,bpt::hours(13)) , bpt::ptime(d,bpt::hours(18))) };
    diffRes = getDiff(mylist2, mylist1);
    ASSERT_EQ(diffRes, expRes);
}

TEST(capa_period, shortCuts)
{
    bg::date d = bg::day_clock::local_day();;

    LCapaPeriod mylist { capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))) };
    LCapaPeriod expRes;

    //Union
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours( 9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(2, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(12)))
            , capa_period(1, bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };
    ASSERT_EQ( getUnion( capa_period(1 , bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))), mylist )
                ,  expRes );
    ASSERT_EQ( getUnion( mylist, capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))) )
                , expRes );
    ASSERT_EQ( getUnion( capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))),
                capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))) )
                , expRes );

    //Intersection
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ( getInter( capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))), mylist )
                , expRes );

    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))) };
    ASSERT_EQ( getInter( mylist, capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))) )
                , expRes );
    ASSERT_EQ( getInter(
            capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))),
            capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))) )
            , expRes );

    //Difference
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(9)) , bpt::ptime(d,bpt::hours(10)))
            , capa_period(1,  bpt::ptime(d,bpt::hours(12)) , bpt::ptime(d,bpt::hours(13))) };
    ASSERT_EQ( getDiff( capa_period(1, bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13))), mylist )
                , expRes );
    expRes = { capa_period(1, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(11)) ) };
    ASSERT_EQ( getDiff( mylist, capa_period(1, bpt::ptime(d,bpt::hours(11)), bpt::ptime(d,bpt::hours(12))) )
                , expRes );
    ASSERT_EQ( getDiff(
        capa_period(1, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12))),
        capa_period(1, bpt::ptime(d,bpt::hours(11)), bpt::ptime(d,bpt::hours(12))))
        , expRes) ;

}

TEST(capa_period, conversion_capa_period)
{
    bg::date d = bg::day_clock::local_day();;

    LCapaPeriod myCapaList { capa_period(10, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)) ) };
    LTimePeriod mylist { time_period( bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) };
    LCapaPeriod expRes { capa_period(10, bpt::ptime(d,bpt::hours(10)) , bpt::ptime(d,bpt::hours(12))) };

    //No union since useless

    //Intersection
    ASSERT_EQ( getInter( myCapaList, time_period( bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(13)) ) )
            ,  expRes );
    ASSERT_EQ( getInter( myCapaList, mylist )
            , expRes );
    ASSERT_EQ( getInter(
                capa_period(10, bpt::ptime(d,bpt::hours(10)), bpt::ptime(d,bpt::hours(12)))
                , time_period(bpt::ptime(d,bpt::hours(9)), bpt::ptime(d,bpt::hours(12))) )
            ,  expRes );

}
