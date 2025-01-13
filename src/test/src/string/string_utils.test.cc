#include <gtest/gtest.h>

#include <nael_utils/string/string_utils.hh>


TEST(string_utils, trim)
{
    ASSERT_EQ("toto", str::trim("toto"));
    ASSERT_EQ("toto", str::trim(" \t\n\r\v\f    toto"));
    ASSERT_EQ("toto", str::trim("toto \t\n\r\v\f    "));
    ASSERT_EQ("toto", str::trim(" \t\n\r\v\f    toto \t\n\r\v\f    "));
}
