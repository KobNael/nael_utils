#include <utils/string/string_utils.hh>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace
{

//test if a char is a space
bool char_isspace(char c) {
    return std::isspace(static_cast<unsigned char>(c));
}
// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun(char_isspace))));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun(char_isspace))).base(), s.end());
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
