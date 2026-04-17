#include <gtest/gtest.h>

#include <nael_utils/exception/exception.hh>

namespace test
{
    MAKE_EXCEPTION(base)
}

namespace
{
    void throw_base()
    {
        throw test::base("dedicated error");
    }
}

TEST(dedicated_exception, assert_throw)
{
    ASSERT_THROW( ::throw_base(), test::base );
    ASSERT_THROW( ::throw_base(), NaelException );
}

TEST(dedicated_exception, get_message)
{
    try
    {
        ::throw_base();
        FAIL();
    }
    catch(const test::base& e)
    {
        ASSERT_EQ( std::string(e.what()), "dedicated error");
    }
}

TEST(dedicated_exception, check_pre)
{
    ASSERT_THROW( CHECK_PRE(false, "check_pre error"), fatal_error );
    ASSERT_NO_THROW( CHECK_PRE(true, "check_pre error") );
}
