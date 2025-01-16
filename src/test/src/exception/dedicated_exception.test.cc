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
        throw test::base("error");
    }
}
TEST(dedicated_exception, basic)
{
    ASSERT_THROW( ::throw_base(), test::base );
    ASSERT_THROW( ::throw_base(), NaelException );
}