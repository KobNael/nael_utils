#pragma once

#include "model_desc_print.hh"
#include "model_desc_att.hh"

#include <boost/describe.hpp>

/**
  @brief allow the declaration of a structure
    MAKE_DTO_STRUCT(
        PressDto,
        (std::string, id)
        (unsigned, pos)
    )
  expands to
    struct PressDto
    {
        std::string id;
        unsigned pos;
    };
 */
#define MAKE_DTO_STRUCT(struct_name, att_seq) \
    struct struct_name \
	{\
	    MAKE_ATT(att_seq) \
	};\
    std::ostream& operator<<(std::ostream& os, struct_name const&obj) \
    {\
        os << BOOST_PP_STRINGIZE(struct_name) << "{";\
        STREAM_ATT_VALUES(att_seq)\
        return os << "}"; \
    }\
	BOOST_DESCRIBE_STRUCT(struct_name, (), (GET_ATT_NAMES(att_seq)))

/**
  @brief Generate every set / get for attributes
    MAKE_CLASS_ATT(
        ( (std::string, id) ), //non editable attributes
        ( (unsigned, x) (unsigned, pos) ) //editable attributes
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
#define MAKE_CLASS_ATT( const_att_seq, editable_att_seq )
    MAKE_CLASS_BASIC_CONST_ATT( const_att_seq ) \
    MAKE_CLASS_BASIC_ATT( editable_att_seq )
