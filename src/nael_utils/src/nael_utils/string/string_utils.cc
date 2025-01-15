#include <nael_utils/string/string_utils.hh>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <string_view>

namespace
{

// trim from start (in place)
void ltrim(std::string &s) {
    std::string_view view(s);
    view.remove_prefix(
        std::distance(
            view.cbegin(),
            std::find_if(view.cbegin(), view.cend(),
            [](char c)
            {
                return !std::isspace(c);
            })));
    s = view;
}

// trim from end (in place)
void rtrim(std::string &s) {
    std::string_view view(s);
    view.remove_suffix(
        std::distance(
            view.crbegin(),
            std::find_if(view.crbegin(), view.crend(),
            [](char c)
            {
                return !std::isspace(c);
            })));
    s = view;
}

// trim from both ends (in place)
void lrtrim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

}

namespace str
{

std::string trim(std::string s) {
    ::lrtrim(s);
    return s;
}

}//namespace str
