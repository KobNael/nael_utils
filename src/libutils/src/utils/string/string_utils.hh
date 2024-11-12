/**
 * @file string_utils
 * @brief Contains some tools for string manipulation
 */

#pragma once

#include <string>

namespace str
{
    /**
     * @brief Trim a string from both ends
     * @param s The string to trim
     * @return a copy of s without any space before (resp. after) the first (resp. last) character
     */
    std::string trim(std::string s);
}
