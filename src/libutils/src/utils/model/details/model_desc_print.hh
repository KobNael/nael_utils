#pragma once

#include "utils/log/log.hh"
#include <boost/preprocessor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
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


}//details

/**@brief stream operator for streamable structure */
std::ostream &operator<<(std::ostream &os, details::streamable const &obj);

#define STREAM_ATT_VALUE_VARIABLE(r, data, i, att_desc) \
	io::print( os << BOOST_PP_IF(i, ", " <<,) BOOST_PP_STRINGIZE( BOOST_PP_SEQ_ELEM(1, att_desc) ) << "=" BOOST_PP_COMMA() \
    BOOST_PP_SEQ_ELEM(1, att_desc) );

#define STREAM_ATT_VALUES(att_seq) \
    BOOST_PP_SEQ_FOR_EACH_I(STREAM_ATT_VALUE_VARIABLE, , att_seq)

