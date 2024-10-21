#pragma once

#include <boost/preprocessor.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <type_traits>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define COMPARE_ATT_VARIABLE(r, data, att) \
    if( safecomp::neq( BOOST_PP_CAT(get_, BOOST_PP_TUPLE_ELEM(0, att))() BOOST_PP_COMMA() other.BOOST_PP_CAT(get_, BOOST_PP_TUPLE_ELEM(0, att))() ) ) \
    {\
        if( safecomp::lt( BOOST_PP_CAT(get_, BOOST_PP_TUPLE_ELEM(0, att))() BOOST_PP_COMMA() other.BOOST_PP_CAT(get_, BOOST_PP_TUPLE_ELEM(0, att))() ) ) \
        {\
            return true; \
        }\
        else \
        {\
            return false;\
        }\
    }

#define COMPARE_ATT_LIST(att_list) \
    BOOST_PP_SEQ_FOR_EACH(COMPARE_ATT_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_list))

#define MAKE_ATT_INIT(add, val) \
    BOOST_PP_IF(add, ={val},);
/**
 @brief Create the declaration of an attribute described as a tuple (type, name)
 */
#define MAKE_ATT_DECL_VARIABLE(r, data, att_desc) \
    BOOST_PP_SEQ_ELEM(0, att_desc) BOOST_PP_SEQ_ELEM(1, att_desc) \
    MAKE_ATT_INIT(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(att_desc), 3),\
        BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(att_desc), 3),BOOST_PP_SEQ_ELEM,BOOST_PP_TUPLE_EAT(2))(2, att_desc) \
    )

/**
 @brief Create the declaration of every attribute in a sequence of attribute description
 */
#define MAKE_ATT_DECLARATION(att_seq) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_DECL_VARIABLE, , att_seq)

/**
 @brief Extract the name of an attribute described as a tuple (type, name)
 adding a separator if needed
 */
#define GET_ATT_NAME_VARIABLE(r, data, i, att_desc) \
    BOOST_PP_COMMA_IF(i) BOOST_PP_SEQ_ELEM(1, att_desc)

/**
 @brief List every attribute in a sequence
 */
#define GET_ATT_NAMES(att_seq) \
    BOOST_PP_SEQ_FOR_EACH_I(GET_ATT_NAME_VARIABLE, , att_seq)

/**
 @brief Create the declaration of an attribute described as a tuple (type, name)
 */
#define MAKE_ATT_SETTER_VARIABLE(r, data, att_desc) \
    void BOOST_PP_CAT(set_, BOOST_PP_SEQ_ELEM(1, att_desc))( BOOST_PP_SEQ_ELEM(0,att_desc) const & val ) \
    { BOOST_PP_SEQ_ELEM(1, att_desc) = val ;}
/**
 @brief Create the getters for every attribute in a sequence of attribute description
 */
#define MAKE_ATT_SETTER(att_seq) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_SETTER_VARIABLE, , att_seq)

/**
 @brief Create the declaration of an attribute described as a tuple (type, name)
 */
#define MAKE_ATT_GETTER_VARIABLE(r, is_const, att_desc) \
    BOOST_PP_SEQ_ELEM(0,att_desc) BOOST_PP_IF( is_const, const , ) &\
    BOOST_PP_CAT(get_, BOOST_PP_SEQ_ELEM(1, att_desc))() BOOST_PP_IF( is_const, const, )\
    { return BOOST_PP_SEQ_ELEM(1, att_desc);}
/**
 @brief Create the getters for every attribute in a sequence of attribute description
 */
#define MAKE_ATT_GETTER(att_seq, is_const) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_GETTER_VARIABLE, 1, att_seq) /* always const getter*/ \
    BOOST_PP_IF(is_const, ,BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_GETTER_VARIABLE, 0, att_seq)) /*editable getter is non const*/

/**
 @brief Generate an editable attribute and every getter/setter
 @param att_seq the sequence of attributes (type, name)
 @param is_const true if the attribute is const and not editable
 */
#define MAKE_MAP_ATT(key, value, name, ordered) \
    private:\
        BOOST_PP_IF(ordered,std::map,std::unordered_map)<key, value> name; \
    public:\
        BOOST_PP_IF(ordered,std::map,std::unordered_map)<key, value> const & BOOST_PP_CAT(get_, name)() const { return name;} \
        BOOST_PP_IF(ordered,std::map,std::unordered_map)<key, value> & BOOST_PP_CAT(get_, name)() { return name;}
/**
 @brief Generate an editable attribute and every getter/setter
 @param att_seq the sequence of attributes (type, name)
 @param is_const true if the attribute is const and not editable
 */
#define MAKE_SET_ATT(value, name, ordered) \
    private:\
        BOOST_PP_IF(ordered,std::set,std::unordered_set)<value> name; \
    public:\
        BOOST_PP_IF(ordered,std::set,std::unordered_set)<value> const & BOOST_PP_CAT(get_, name)() const { return name;} \
        BOOST_PP_IF(ordered,std::set,std::unordered_set)<value> & BOOST_PP_CAT(get_, name)() { return name;}

/**
 @brief Generate an editable attribute and every getter/setter
 @param att_seq the sequence of attributes (type, name)
 @param is_const true if the attribute is const and not editable
 */
#define MAKE_CLASS_BASIC_ATT(att_seq, is_const) \
    BOOST_PP_IF( BOOST_PP_SEQ_SIZE(att_seq) \
    ,\
        private: MAKE_ATT_DECLARATION(att_seq) \
        public: MAKE_ATT_GETTER(att_seq, is_const) \
        BOOST_PP_IF(is_const, , public: MAKE_ATT_SETTER(att_seq)) \
    , )

