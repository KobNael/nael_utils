#include <nael_utils/string/string_utils.hh>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

#include <format>
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
    bool get_val_from_str<bool>(const std::string &str_val)
    {
        static auto true_values = {"1", "true", "True", "TRUE"};
        static auto false_values = {"0", "false", "False", "FALSE"};
        if(std::ranges::find(true_values, str_val) != true_values.end())
            return true;
        else if(std::ranges::find(false_values, str_val) != false_values.end())
            return false;
        else
            throw bad_lexical_cast(std::format("Cannot parse value {}", str_val));
    }

    template <>
    bg::date get_val_from_str<bg::date>(const std::string &str_val)
    {
        try
        {
            return bg::date_from_iso_string(str_val);
        }
        // date_from_iso_string can throw either boost::bad_lexical_cast if the format is invalid
        catch(boost::bad_lexical_cast const&e)
        {
            throw bad_lexical_cast(
                std::format("Cannot parse value {} : {}", str_val, e.what()));
        }
        // or std::out_of_range if the date is out of range (e.g. month > 12)
        catch(std::out_of_range const &e)
        {
            throw bad_lexical_cast(
                std::format("Cannot parse value {} : {}", str_val, e.what()));
        }
    }

    template <>
    bpt::ptime get_val_from_str<bpt::ptime>(const std::string &str_val)
    {
        try
        {
            return bpt::from_iso_extended_string(str_val);
        }
        // from_iso_extended_string can throw either boost::bad_lexical_cast if the format is invalid
        catch(boost::bad_lexical_cast const&e)
        {
            throw bad_lexical_cast(
                std::format("Cannot parse value {} : {}", str_val, e.what()));
        }
        // or std::out_of_range if the date is out of range (e.g. month > 12)
        catch(std::out_of_range const &e)
        {
            throw bad_lexical_cast(
                std::format("Cannot parse value {} : {}", str_val, e.what()));
        }
    }

    template <>
    bpt::time_duration get_val_from_str<bpt::time_duration>(const std::string &str_val)
    {
        try
        {
            return bpt::duration_from_string(str_val);
        }
        // duration_from_string can throw either boost::bad_lexical_cast if the format is invalid
        catch(boost::bad_lexical_cast const&e)
        {
            throw bad_lexical_cast(
                std::format("Cannot parse value {} : {}", str_val, e.what()));
        }
        // or std::out_of_range on empty strings
        catch(std::out_of_range const &e)
        {
            throw bad_lexical_cast(
                std::format("Cannot parse value {} : {}", str_val, e.what()));
        }
    }

} // namespace str
