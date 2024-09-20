#pragma once

#include <boost/preprocessor.hpp>
#include <iostream>
#include <vector>

namespace details
{

/**
 * @brief streamable structure
 */
struct streamable
{
    virtual std::ostream &stream(std::ostream &os) const = 0;
};

std::ostream &operator<<(std::ostream &os, streamable const &obj);

/**
 * @brief Print an object in a stream
 * @param os the ostream
 * @param obj the object
 */
template<typename T>
std::ostream &print(std::ostream &os, T const &obj)
{
    return os << obj;
}
/**
 * @brief Print every object from a vector in a stream
 * @param os the ostream
 * @param vec the vector of object
 */
template<typename T>
std::ostream &print(std::ostream &os, std::vector<T> const &vec)
{
    os << "[";
    bool first{true};
    for(T const &obj : vec)
    {
        os << ((first)?"":",") << obj;
        first = false;
    }
    return os << "]";
}
}//details

#define STREAM_ATT_VALUE_VARIABLE(r, data, i, att_desc) \
	details::print( os << BOOST_PP_IF(i, ", " <<,) BOOST_PP_STRINGIZE( BOOST_PP_TUPLE_ELEM(1, att_desc) ) << "=" BOOST_PP_COMMA() \
    BOOST_PP_TUPLE_ELEM(1, att_desc) );

#define STREAM_ATT_VALUES(att_seq) \
    BOOST_PP_SEQ_FOR_EACH_I(STREAM_ATT_VALUE_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq))

