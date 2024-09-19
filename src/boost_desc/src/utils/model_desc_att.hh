#pragma once

#include <boost/preprocessor.hpp>


/**
 @brief Create the declaration of an attribute described as a tuple (type, name)
 */
#define MAKE_ATT_VARIABLE(r, data, att_desc) \
	BOOST_PP_TUPLE_ELEM(0, att_desc) BOOST_PP_TUPLE_ELEM(1,att_desc);

/**
 @brief Create the declaration of every attribute in a sequence of attribute description
 */
#define MAKE_ATT(att_seq) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq))

/**
 @brief Extract the name of an attribute described as a tuple (type, name)
 adding a separator if needed
 */
#define GET_ATT_NAME_VARIABLE(r, data, i, att_desc) \
	BOOST_PP_COMMA_IF(i) BOOST_PP_TUPLE_ELEM(1, att_desc)

/**
 @brief Extract the name of each attributes in a sequence of attribute description
 */
#define GET_ATT_NAMES(att_seq) \
    BOOST_PP_SEQ_FOR_EACH_I(GET_ATT_NAME_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq))

#define MAKE_CLASS_BASIC_ATT(att_seq)

#define MAKE_CLASS_BASIC_CONST_ATT(att_seq)