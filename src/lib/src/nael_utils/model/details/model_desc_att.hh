/**
 * @file model_desc_att.hh
 */
#pragma once

#include <boost/preprocessor.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <type_traits>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <nael_utils/hash/hash.hh>


/**
 * @brief Extract the name of an attribute described as a tuple (type, name) adding a separator if needed
 * @param r
 * @param data
 * @param i
 * @param att_desc the tuple
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT and #MAKE_DTO_STRUCT
 */
#define GET_DTO_ATT_NAME_VARIABLE(r, data, i, att_desc) \
    BOOST_PP_COMMA_IF(i) BOOST_PP_SEQ_ELEM(1, att_desc)

/**
 * @brief Extract every attribute names from a sequence of attribute description
 * @param att_seq the sequence of attributes
 * @warning This macro should not be called directly, it is used through #MAKE_DTO_STRUCT
 */
#define GET_DTO_ATT_NAMES(att_seq) \
    BOOST_PP_SEQ_FOR_EACH_I(GET_DTO_ATT_NAME_VARIABLE, , att_seq)

/**
 * @brief Generate a safe comparison on an attribute
 * @param r
 * @param data
 * @param att name of the attribute
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_SORT
 */
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

/**
 * @brief Generate a safe comparison on a sequence of attributes
 * @param att_seq
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_SORT
 */
#define COMPARE_ATT_LIST(att_seq) \
    BOOST_PP_SEQ_FOR_EACH(COMPARE_ATT_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_seq))

/**
 * @brief Generate an initialization of an attribute value
 * @param add should we add the initialization
 * @param val the value
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT and #MAKE_DTO_STRUCT
 */
#define MAKE_ATT_INIT(add, val) \
    BOOST_PP_IF(add, ={val},);
/**
 * @brief Create the declaration of an attribute described as a tuple (type, name)
 * @param r
 * @param data
 * @param att_desc the tuple
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT and #MAKE_DTO_STRUCT
 */
#define MAKE_DTO_ATT_DECL_VARIABLE(r, data, att_desc) \
    BOOST_PP_SEQ_ELEM(0, att_desc) BOOST_PP_SEQ_ELEM(1, att_desc) \
    MAKE_ATT_INIT(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(att_desc), 3),\
        BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(att_desc), 3),BOOST_PP_SEQ_ELEM,BOOST_PP_TUPLE_EAT(2))(2, att_desc) \
    )

/**
 * @brief Create the declaration of an attribute described as a tuple (type, name)
 * @param r
 * @param data
 * @param att_desc the tuple
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT and #MAKE_DTO_STRUCT
 */
#define MAKE_CLASS_ATT_DECL_VARIABLE(r, data, att_desc) \
    BOOST_PP_SEQ_ELEM(0, att_desc) BOOST_PP_CAT(_, BOOST_PP_SEQ_ELEM(1, att_desc)) \
    MAKE_ATT_INIT(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(att_desc), 3),\
        BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(att_desc), 3),BOOST_PP_SEQ_ELEM,BOOST_PP_TUPLE_EAT(2))(2, att_desc) \
    )

/**
 * @brief Create the declaration of every attribute in a sequence of attribute description
 * @param att_seq the sequence of attributes
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT and #MAKE_DTO_STRUCT
 */
#define MAKE_ATT_DECLARATION(class, att_seq) \
    BOOST_PP_IF(class, \
        BOOST_PP_SEQ_FOR_EACH(MAKE_CLASS_ATT_DECL_VARIABLE, , att_seq), \
        BOOST_PP_SEQ_FOR_EACH(MAKE_DTO_ATT_DECL_VARIABLE, , att_seq) )

/**
 * @brief Create the declaration of the setter of an attribute
 * @param r
 * @param data
 * @param att_desc the attribute described as sequence (type)(name)
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT
 */
#define MAKE_ATT_SETTER_VARIABLE(r, data, att_desc) \
    void BOOST_PP_CAT(set_, BOOST_PP_SEQ_ELEM(1, att_desc))( BOOST_PP_SEQ_ELEM(0,att_desc) const & val ) \
    { BOOST_PP_CAT(_, BOOST_PP_SEQ_ELEM(1, att_desc)) = val ;}
/**
 * @brief Create the declaration of the setters for every attribute in a sequence of attribute description
 * @param att_seq the sequence of attributes
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT
 */
#define MAKE_ATT_SETTER(att_seq) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_SETTER_VARIABLE, , att_seq)

/**
 * @brief Create the declaration of the getter(s) of an attribute
 * @param r
 * @param is_const is the attribute const (if not a getter type& get_att() will be created)
 * @param att_desc the attribute described as sequence (type)(name)
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT
 */
#define MAKE_ATT_GETTER_VARIABLE(r, is_const, att_desc) \
    BOOST_PP_SEQ_ELEM(0,att_desc) BOOST_PP_IF( is_const, const , ) &\
    BOOST_PP_CAT(get_, BOOST_PP_SEQ_ELEM(1, att_desc))() BOOST_PP_IF( is_const, const, )\
    { return BOOST_PP_CAT(_, BOOST_PP_SEQ_ELEM(1, att_desc));}
/**
 * @brief Create the declaration of the getters for every attribute in a sequence of attribute description
 * @param is_const is the attribute const (if not a getter type& get_att() will be created)
 * @param att_seq the sequence of attributes
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT
 */
#define MAKE_ATT_GETTER(att_seq, is_const) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_GETTER_VARIABLE, 1, att_seq) /* always const getter*/ \
    BOOST_PP_IF(is_const, ,BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_GETTER_VARIABLE, 0, att_seq)) /*editable getter is non const*/

/**
 * @brief Generate an editable attribute `std::map<key, value> _name` and every getter/setter
 * @param key the type of the keys
 * @param value the type of the values
 * @param name the name of the attribute
 * @warning This macro should not be called directly, it is used through #MAKE_MAP
 */
#define MAKE_MAP_ATT(key, value, name) \
    private:\
        std::map<key, value> BOOST_PP_CAT(_, name); \
    public:\
        std::map<key, value> const & BOOST_PP_CAT(get_, name)() const { return BOOST_PP_CAT(_, name);} \
        std::map<key, value> & BOOST_PP_CAT(get_, name)() { return BOOST_PP_CAT(_, name);}
/**
 * @brief Generate an editable attribute `std::set<value> _name` and every getter/setter
 * @param value the type of the values
 * @param name the name of the attribute
 * @warning This macro should not be called directly, it is used through #MAKE_SET
 */
#define MAKE_SET_ATT(value, name) \
    private:\
        std::set<value> BOOST_PP_CAT(_, name); \
    public:\
        std::set<value> const & BOOST_PP_CAT(get_, name)() const { return BOOST_PP_CAT(_, name);} \
        std::set<value> & BOOST_PP_CAT(get_, name)() { return BOOST_PP_CAT(_, name);}

/**
 * @brief Generate an editable attribute `std::unordered_map<std::string, value> _name` and every getter/setter
 * @param value the type of the values
 * @param name the name of the attribute
 * @warning This macro should not be called directly, it is used through #MAKE_STRHASH_MAP
 */
#define MAKE_STRHASH_MAP_ATT(value, name) \
    private:\
        std::unordered_map<std::string, value, string_hash, std::equal_to<>> BOOST_PP_CAT(_, name); \
    public:\
        std::unordered_map<std::string, value, string_hash, std::equal_to<>> const & BOOST_PP_CAT(get_, name)() const { return BOOST_PP_CAT(_, name);} \
        std::unordered_map<std::string, value, string_hash, std::equal_to<>> & BOOST_PP_CAT(get_, name)() { return BOOST_PP_CAT(_, name);}
/**
 * @brief Generate an editable attribute `type  &_name` and every getter/setter
 * @param type the class name
 * @param name the name of the attribute
 * @param is_const is the reference const
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_REF_ATT and #MAKE_CLASS_CONSTREF_ATT
 */
#define MAKE_BASIC_CLASS_REF_ATT(type, name, is_const) \
    private:\
    std::reference_wrapper<type BOOST_PP_IF(is_const, const ,)> BOOST_PP_CAT( _, name); \
    public:\
        type const& BOOST_PP_CAT(get_, name)() const { return BOOST_PP_CAT(_, name).get();} \
        BOOST_PP_IF(is_const, , \
            type & BOOST_PP_CAT(get_, name)() { return BOOST_PP_CAT(_, name.get());} \
        )


/**
 * @brief Generate every declaration and getters/setters for a sequence of attributes
 * each attribute being a sequence (type)(name)
 * @param att_seq the sequence of attributes
 * @param is_const true if the attribute is const and not editable
 * @warning This macro should not be called directly, it is used through #MAKE_CLASS_ATT
 */
#define MAKE_CLASS_BASIC_ATT(att_seq, is_const) \
    BOOST_PP_IF( BOOST_PP_SEQ_SIZE(att_seq) \
    ,\
        private: MAKE_ATT_DECLARATION(1, att_seq) \
        public: MAKE_ATT_GETTER(att_seq, is_const) \
        BOOST_PP_IF(is_const, , public: MAKE_ATT_SETTER(att_seq)) \
    , )
