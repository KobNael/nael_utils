#pragma once

#include <boost/preprocessor.hpp>
#include <type_traits>

/**
 @brief Create the declaration of an attribute described as a tuple (type, name)
 */
#define MAKE_ATT_DECL_VARIABLE(r, data, att_desc) \
    BOOST_PP_TUPLE_ELEM(0, att_desc) BOOST_PP_TUPLE_ELEM(1, att_desc);

/**
 @brief Create the declaration of every attribute in a sequence of attribute description
 */
#define MAKE_ATT_DECLARATION(att_seq) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_DECL_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq))

/**
 @brief Extract the name of an attribute described as a tuple (type, name)
 adding a separator if needed
 */
#define GET_ATT_NAME_VARIABLE(r, data, i, att_desc) \
    BOOST_PP_COMMA_IF(i) BOOST_PP_TUPLE_ELEM(1, att_desc)

/**
 @brief List every attribute in a sequence
 */
#define GET_ATT_NAMES(att_seq) \
    BOOST_PP_SEQ_FOR_EACH_I(GET_ATT_NAME_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq))



/**
 @brief Create the declaration of an attribute described as a tuple (type, name)
 */
#define MAKE_ATT_SETTER_VARIABLE(r, data, att_desc) \
    void BOOST_PP_CAT(set_, BOOST_PP_TUPLE_ELEM(1, att_desc))( BOOST_PP_TUPLE_ELEM(0,att_desc) const & val ) \
    { BOOST_PP_TUPLE_ELEM(1, att_desc) = val ;}
/**
 @brief Create the getters for every attribute in a sequence of attribute description
 */
#define MAKE_ATT_SETTER(att_seq) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_SETTER_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq))

/**
 @brief Create the declaration of an attribute described as a tuple (type, name)
 */
#define MAKE_ATT_GETTER_VARIABLE(r, is_const, att_desc) \
    BOOST_PP_TUPLE_ELEM(0,att_desc) BOOST_PP_IF( is_const, const , ) &\
    BOOST_PP_CAT(get_, BOOST_PP_TUPLE_ELEM(1, att_desc))() BOOST_PP_IF( is_const, const, )\
    { return BOOST_PP_TUPLE_ELEM(1, att_desc);}
/**
 @brief Create the getters for every attribute in a sequence of attribute description
 */
#define MAKE_ATT_GETTER(att_seq, is_const) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_GETTER_VARIABLE, 1, BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq)) /* always const getter*/ \
    BOOST_PP_IF(is_const, ,BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_GETTER_VARIABLE, 0, BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq))) /*editable getter is non const*/

//#define MAKE_CLASS_ATT_VARIABLE(r, data, i, att_desc)
//    private: MAKE_ATT_DECLARATION()
//    BOOST_PP_TUPLE_ELEM(0, att_desc) BOOST_PP_TUPLE_ELEM(1,att_desc);


/**
 @brief Generate an editable attribute and every getter/setter
 @param att_seq the sequence of attributes (type, name)
 @param is_const true if the attribute is const and not editable
 */
#define MAKE_CLASS_BASIC_ATT(att_seq, is_const) \
    BOOST_PP_IF(BOOST_PP_SEQ_SIZE(BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq)),\
    private: MAKE_ATT_DECLARATION(att_seq) \
    public: MAKE_ATT_GETTER(att_seq, is_const) \
    BOOST_PP_IF(is_const, , public: MAKE_ATT_SETTER(att_seq)) \
    , )

