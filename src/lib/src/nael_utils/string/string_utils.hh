/**
 * @file string_utils
 * @brief Contains some tools for string manipulation
 */

#pragma once

#include <nael_utils/exception/exception.hh>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <string>

namespace bg = boost::gregorian;

namespace str
{

    /**
     * @brief Dedicated exception for cast error from (resp. to) string
     */
    MAKE_EXCEPTION(bad_lexical_cast)

    /**
     * @brief Read a value from a string (using boost::lexical_cast)
     * @return the value
     * @throw bad_lexical_cast if the value can not be parsed
     */
    template<typename T>
    T get_val_from_str(const std::string &str_val)
    {
        try{
            T val_l = boost::lexical_cast<T>( str_val );
            return val_l;
        }
        catch ( const boost::bad_lexical_cast& ) {
            throw bad_lexical_cast( "Cannot parse value : " + str_val);
        }
    }

    /**
     * @brief Read a value from a string (using boost::lexical_cast)
     * @return the value
     * @throw bad_lexical_cast if the value can not be parsed
     */
    template<>
    bg::date get_val_from_str<bg::date>(const std::string &str_val);

    /**
     * @brief Trim a string from both ends
     * @param s The string to trim
     * @return a copy of s without any space before (resp. after) the first (resp. last) character
     */
    std::string trim(std::string s);
}
