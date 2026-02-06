#include <gtest/gtest.h>

#include <nael_utils/string/string_utils.hh>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace bg = boost::gregorian;
namespace bpt = boost::posix_time;

TEST(string_utils, trim)
{
    ASSERT_EQ("toto", str::trim("toto"));
    ASSERT_EQ("toto", str::trim(" \t\n\r\v\f    toto"));
    ASSERT_EQ("toto", str::trim("toto \t\n\r\v\f    "));
    ASSERT_EQ("toto", str::trim(" \t\n\r\v\f    toto \t\n\r\v\f    "));

    // Edge cases
    ASSERT_EQ("", str::trim(""));
    ASSERT_EQ("", str::trim(" \t\n\r\v\f"));
    ASSERT_EQ("a", str::trim("a"));
    ASSERT_EQ("a", str::trim(" a "));
    ASSERT_EQ("hello world", str::trim("  hello world  "));
    ASSERT_EQ("test\nwith\nnewlines", str::trim(" \t test\nwith\nnewlines \n "));
}

TEST(string_utils, get_val_from_str_integer)
{
    ASSERT_EQ(42, str::get_val_from_str<int>("42"));
    ASSERT_EQ(-123, str::get_val_from_str<int>("-123"));
    ASSERT_EQ(0, str::get_val_from_str<int>("0"));

    // Long integers
    ASSERT_EQ(1234567890L, str::get_val_from_str<long>("1234567890"));

    // Invalid integer strings should throw
    ASSERT_THROW(str::get_val_from_str<int>("abc"), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<int>("12.34"), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<int>(""), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<int>("12abc"), str::bad_lexical_cast);
}

TEST(string_utils, get_val_from_str_floating_point)
{
    ASSERT_DOUBLE_EQ(3.14159, str::get_val_from_str<double>("3.14159"));
    ASSERT_DOUBLE_EQ(-2.5, str::get_val_from_str<double>("-2.5"));
    ASSERT_DOUBLE_EQ(0.0, str::get_val_from_str<double>("0.0"));
    ASSERT_DOUBLE_EQ(1.0, str::get_val_from_str<double>("1"));

    // Scientific notation
    ASSERT_DOUBLE_EQ(1.5e10, str::get_val_from_str<double>("1.5e10"));
    ASSERT_DOUBLE_EQ(2.3e-5, str::get_val_from_str<double>("2.3e-5"));

    // Float type
    ASSERT_FLOAT_EQ(1.5f, str::get_val_from_str<float>("1.5"));

    // Invalid float strings should throw
    ASSERT_THROW(str::get_val_from_str<double>("not_a_number"), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<double>("3.14.15"), str::bad_lexical_cast);
}

TEST(string_utils, get_val_from_str_boolean)
{
    ASSERT_TRUE(str::get_val_from_str<bool>("1"));
    ASSERT_TRUE(str::get_val_from_str<bool>("true"));
    ASSERT_TRUE(str::get_val_from_str<bool>("True"));
    ASSERT_TRUE(str::get_val_from_str<bool>("TRUE"));
    ASSERT_FALSE(str::get_val_from_str<bool>("0"));
    ASSERT_FALSE(str::get_val_from_str<bool>("false"));
    ASSERT_FALSE(str::get_val_from_str<bool>("False"));
    ASSERT_FALSE(str::get_val_from_str<bool>("FALSE"));

    // Invalid boolean strings should throw
    ASSERT_THROW(str::get_val_from_str<bool>("yes"), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<bool>("no"), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<bool>("2"), str::bad_lexical_cast);
}

TEST(string_utils, get_val_from_str_string)
{
    ASSERT_EQ("hello", str::get_val_from_str<std::string>("hello"));
    ASSERT_EQ("", str::get_val_from_str<std::string>(""));
    ASSERT_EQ("123", str::get_val_from_str<std::string>("123"));
    ASSERT_EQ("with spaces", str::get_val_from_str<std::string>("with spaces"));
}

TEST(string_utils, get_val_from_str_date)
{
    // Test valid ISO date strings
    bg::date expected_date(2023, 12, 25);
    ASSERT_EQ(expected_date, str::get_val_from_str<bg::date>("20231225"));

    bg::date expected_date2(2024, 1, 1);
    ASSERT_EQ(expected_date2, str::get_val_from_str<bg::date>("20240101"));

    // Invalid date strings should throw
    ASSERT_THROW(str::get_val_from_str<bg::date>("invalid_date"), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<bg::date>("2023-12-25"), str::bad_lexical_cast); // Wrong format
    ASSERT_THROW(str::get_val_from_str<bg::date>(""), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<bg::date>("20231301"), str::bad_lexical_cast); // Invalid month
}

TEST(string_utils, get_val_from_str_ptime)
{
    // Test valid ISO extended time strings
    bpt::ptime expected_time(bg::date(2023, 12, 25), bpt::hours(14) + bpt::minutes(30) + bpt::seconds(45));
    ASSERT_EQ(expected_time, str::get_val_from_str<bpt::ptime>("2023-12-25T14:30:45"));

    bpt::ptime expected_time2(bg::date(2024, 1, 1), bpt::hours(0) + bpt::minutes(0) + bpt::seconds(0));
    ASSERT_EQ(expected_time2, str::get_val_from_str<bpt::ptime>("2024-01-01T00:00:00"));

    bpt::ptime expected_time3(bg::date(2023, 12, 26), bpt::hours(1) + bpt::minutes(30) + bpt::seconds(45));
    ASSERT_EQ(expected_time3, str::get_val_from_str<bpt::ptime>("2023-12-25T25:30:45"));

    // Invalid time strings should throw
    ASSERT_THROW(str::get_val_from_str<bpt::ptime>("invalid_time"), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<bpt::ptime>("2023/12/25 14:30:45"), str::bad_lexical_cast); // Wrong format
    ASSERT_THROW(str::get_val_from_str<bpt::ptime>(""), str::bad_lexical_cast);
}

TEST(string_utils, get_val_from_str_time_duration)
{
    // Test valid duration strings
    bpt::time_duration expected_duration1 = bpt::hours(2) + bpt::minutes(30) + bpt::seconds(15);
    ASSERT_EQ(expected_duration1, str::get_val_from_str<bpt::time_duration>("02:30:15"));

    bpt::time_duration expected_duration2 = bpt::hours(0) + bpt::minutes(5) + bpt::seconds(0);
    ASSERT_EQ(expected_duration2, str::get_val_from_str<bpt::time_duration>("00:05:00"));

    bpt::time_duration expected_duration3 = bpt::hours(23) + bpt::minutes(59) + bpt::seconds(59);
    ASSERT_EQ(expected_duration3, str::get_val_from_str<bpt::time_duration>("23:59:59"));

    bpt::time_duration expected_duration4 = bpt::hours(25) + bpt::minutes(30) + bpt::seconds(15);
    ASSERT_EQ(expected_duration4, str::get_val_from_str<bpt::time_duration>("25:30:15"));

    bpt::time_duration expected_duration5 = bpt::hours(250);
    ASSERT_EQ(expected_duration5, str::get_val_from_str<bpt::time_duration>("250"));

    // Invalid duration strings should throw
    ASSERT_THROW(str::get_val_from_str<bpt::time_duration>("invalid_duration"), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<bpt::time_duration>("2h 30m"), str::bad_lexical_cast); // Wrong format
    ASSERT_THROW(str::get_val_from_str<bpt::time_duration>(""), str::bad_lexical_cast);
}

TEST(string_utils, get_val_from_str_edge_cases)
{
    // Test with whitespace (should fail for most types as boost::lexical_cast is strict)
    ASSERT_THROW(str::get_val_from_str<int>(" 42 "), str::bad_lexical_cast);
    ASSERT_THROW(str::get_val_from_str<double>(" 3.14 "), str::bad_lexical_cast);

    // String should handle whitespace
    ASSERT_EQ(" 42 ", str::get_val_from_str<std::string>(" 42 "));
}