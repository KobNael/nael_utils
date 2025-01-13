#include <gtest/gtest.h>

#include <nael_utils/safe_comp/safe_comp.hh>

TEST(safe_comp, testround)
{
    EXPECT_EQ(safecomp::round<double>(2.3),2.0);
    EXPECT_EQ(safecomp::round<double>(-1.8), -2.0);
}

TEST(safe_comp, testLt)
{
    EXPECT_TRUE(safecomp::lt<double>(0,1));
    EXPECT_FALSE(safecomp::lt<double>(0,1,2));
}

TEST(safe_comp, testGt)
{
    EXPECT_TRUE(safecomp::gt<double>(2,1));
    EXPECT_FALSE(safecomp::gt<double>(0,1));
    EXPECT_FALSE(safecomp::gt<double>(1,0,2));
}

TEST(safe_comp, testEq)
{
    EXPECT_FALSE(safecomp::eq<double>(0,1));
    EXPECT_TRUE(safecomp::eq<double>(0,1,1));
    EXPECT_TRUE(safecomp::eq<double>(1e-7,0));
}

TEST(safe_comp, testlE)
{
    EXPECT_TRUE(safecomp::le<double>(0,1));
    EXPECT_TRUE(safecomp::le<double>(0,1,1));
    EXPECT_FALSE(safecomp::le<double>(1e-5,0));
}

TEST(safe_comp, testgE)
{
    EXPECT_FALSE(safecomp::ge<double>(0,1));
    EXPECT_TRUE(safecomp::ge<double>(0,1,1));
}

TEST(safe_comp, testnEq)
{
    EXPECT_TRUE(safecomp::neq<double>(0,1));
    EXPECT_FALSE(safecomp::neq<double>(0,1,1));
    EXPECT_FALSE(safecomp::neq<double>(1e-6,0));
}

TEST(safe_comp, testisNull)
{
    EXPECT_TRUE(safecomp::isnull<double>(0));
    EXPECT_FALSE(safecomp::isnull<double>(1));
    EXPECT_TRUE(safecomp::isnull<double>(1,2));
}

TEST(safe_comp, testisNeg)
{
    EXPECT_TRUE(safecomp::isneg<double>(-1));
    EXPECT_FALSE(safecomp::isneg<double>(-1,2));
}

TEST(safe_comp, testisPos)
{
    EXPECT_TRUE(safecomp::ispos<double>(1));
    EXPECT_FALSE(safecomp::ispos<double>(1,2));
}

TEST(safe_comp, testisInt)
{
    EXPECT_TRUE(safecomp::isint<double>(1));
    EXPECT_FALSE(safecomp::isint<double>(1.4));
    EXPECT_TRUE(safecomp::isint<double>(1.4,0.5));
}

TEST(safe_comp, testisNan)
{
    EXPECT_TRUE(safecomp::isnan<double>(log(-1)));
    EXPECT_FALSE(safecomp::isnan<double>(2));
}

TEST(safe_comp, testisInf)
{
    EXPECT_TRUE(safecomp::isinf(std::numeric_limits<double>::infinity()));
}
