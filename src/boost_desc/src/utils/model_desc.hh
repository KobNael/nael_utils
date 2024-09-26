#pragma once

#include "model_desc_print.hh"
#include "model_desc_att.hh"

#include <boost/describe.hpp>
#include<vector>

#define COMPARE_ATT_VARIABLE(r, data, att) \
    if( BOOST_PP_CAT(get_, BOOST_PP_TUPLE_ELEM(0, att))() < other.BOOST_PP_CAT(get_, BOOST_PP_TUPLE_ELEM(0, att))()) \
    { \
        return true; \
    }

#define COMPARE_ATT_LIST(att_list) \
    BOOST_PP_SEQ_FOR_EACH(COMPARE_ATT_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(att_list))

/**
  @brief Create an operator< based on a list of attributes
    MAKE_CLASS_SORT(AbstractClosing, (from, to) )
  expands to
    bool operator<(AbstractClosing const&other) const
    {
        if( get_from() < other.get_from()) { return true; }
        if( get_to() < other.get_to()) { return true; }
        return false;
    }
*/
#define MAKE_CLASS_SORT(class_name, att_list) \
public: \
    bool operator<(class_name const&other) const\
    {\
        COMPARE_ATT_LIST(BOOST_PP_TUPLE_TO_SEQ(att_list))\
        return false;\
    }


/**
  @brief allow the declaration of a structure
    MAKE_DTO_STRUCT(
        PressDto,
        ((std::string) (id))
        ((unsigned) (pos))
    )
  expands to
    struct PressDto
    {
        std::string id;
        unsigned pos;
    };
 */
#define MAKE_DTO_STRUCT(struct_name, att_seq) \
    struct struct_name : public details::streamable\
    {\
        virtual ~struct_name(){} \
        MAKE_ATT_DECLARATION(att_seq) \
        std::ostream& stream(std::ostream& os) const override\
        {\
            os << BOOST_PP_STRINGIZE(struct_name) << "{";\
            STREAM_ATT_VALUES(att_seq)\
            return os << "}"; \
        }\
    };\
    BOOST_DESCRIBE_STRUCT(struct_name, (), (GET_ATT_NAMES(att_seq)))

/**
  @brief Generate every set / get for attributes
    MAKE_CLASS_ATT(
        //non editable attributes
        ( (std::string)(id) ),
        //editable attributes
        ( (unsigned)(x) )
        ( (unsigned)(pos) )
    )
  expands to
    private:
        std::string id;
    public:
        std::string const &get_id() const { return id; }
    private:
        unsigned x;
    public:
        void set_x(unsigned p) { x = p; }
        unsigned &get_x() { return x; }
        unsigned get_x() const { return x; }
    private:
        unsigned pos;
    public:
        void set_pos(unsigned p) { pos = p; }
        unsigned &get_pos() { return pos; }
        unsigned get_pos() const { return pos; }
 */
#define MAKE_CLASS_ATT( const_att_seq, editable_att_seq ) \
    MAKE_CLASS_BASIC_ATT( const_att_seq, 1 ) \
    MAKE_CLASS_BASIC_ATT( editable_att_seq, 0 )
