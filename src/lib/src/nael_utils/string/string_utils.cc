#include <nael_utils/string/string_utils.hh>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <string_view>
#include <ranges>

namespace
{

    // trim from start (in place)
    void ltrim(std::string &s)
    {
        std::string_view view(s);
        view.remove_prefix(
            std::distance(
                view.cbegin(),
                std::ranges::find_if(view, [](char c)
                                     { return !std::isspace(c); })));
        s = view;
    }

    // trim from end (in place)
    void rtrim(std::string &s)
    {
        std::string_view view(s);
        view.remove_suffix(
            std::distance(
                view.crbegin(),
                std::ranges::find_if(view | std::views::reverse, [](char c)
                                     { return !std::isspace(c); })));
        s = view;
    }

    // trim from both ends (in place)
    void lrtrim(std::string &s)
    {
        ltrim(s);
        rtrim(s);
    }

} // namespace

namespace str
{

    std::string trim(std::string s)
    {
        ::lrtrim(s);
        return s;
    }

    template <>
    bg::date get_val_from_str<bg::date>(const std::string &str_val)
    {
        return bg::date_from_iso_string(str_val);
    }

    template <>
    bpt::ptime get_val_from_str<bpt::ptime>(const std::string &str_val)
    {
        return bpt::from_iso_extended_string(str_val);
    }

} // namespace str
