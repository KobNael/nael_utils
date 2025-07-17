/**
 * @file model_desc_print.hh
 */

#pragma once

#include <nael_utils/model/details/model_streamable.hh>
#include <nael_utils/log/log.hh>

#include <boost/preprocessor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <vector>

/**
 * @brief Generate the instruction needed to stream an attribute
 * @warning This macro should not be called directly, it is used through #MAKE_DTO_STRUCT
 */
#define STREAM_DTO_ATT_VALUE_VARIABLE(r, data, i, att_desc)                                                             \
    io::print(os << BOOST_PP_IF(i, ", " <<, ) BOOST_PP_STRINGIZE( BOOST_PP_SEQ_ELEM(1, att_desc) ) << "=" BOOST_PP_COMMA() \
                                                                                       BOOST_PP_SEQ_ELEM(1, att_desc));

/**
 * @brief Generate the instructions needed to stream a sequence of attributes
 * @param att_seq the sequence
 * @warning This macro should not be called directly, it is used through #MAKE_DTO_STRUCT
 */
#define STREAM_DTO_ATT_VALUES(att_seq) \
    BOOST_PP_SEQ_FOR_EACH_I(STREAM_DTO_ATT_VALUE_VARIABLE, , att_seq)
