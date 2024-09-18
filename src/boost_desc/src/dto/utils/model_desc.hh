#pragma once

#include <boost/preprocessor.hpp>
#include <boost/describe.hpp>


#define STREAM_ATT_NAME_VARIABLE(r, data, i, elem) \
	BOOST_PP_IF(i, << ", " <<,)\
    BOOST_PP_STRINGIZE( BOOST_PP_TUPLE_ELEM(1, elem) ) << "=" << obj.BOOST_PP_TUPLE_ELEM(1, elem)

#define STREAM_ATT_VALUES(seq) \
    BOOST_PP_SEQ_FOR_EACH_I(STREAM_ATT_NAME_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(seq))

/**
 @brief Create the declaration of every attribute in a sequence
 */
#define MAKE_ATT_VARIABLE(r, data, elem) \
	BOOST_PP_TUPLE_ELEM(0, elem) BOOST_PP_TUPLE_ELEM(1,elem);
/**
 @brief Create the declaration of every attribute in a sequence
 */
#define MAKE_ATT(seq) \
    BOOST_PP_SEQ_FOR_EACH(MAKE_ATT_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(seq))

/**
 @brief Extract the name of an attribute, adding a separator if needed
 */
#define GET_ATT_NAME_VARIABLE(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) BOOST_PP_TUPLE_ELEM(1, elem)
/**
 @brief Extract the name of each attributes in the sequence (att1, att2)
 */
#define GET_ATT_NAMES(seq) \
    BOOST_PP_SEQ_FOR_EACH_I(GET_ATT_NAME_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(seq))

/**
  @brief allow the declaration of a structure

  MAKE_DTO_STRUCT(
 	PressDto,
 	(std::string, id, identifier)
 	(unsigned, pos, relative position)
  )

  expands to

  struct PressDto
  {
    std::string id; //identifier
    unsigned pos; //relative position
  };

 */
#define MAKE_DTO_STRUCT(STRUCT_NAME, ATT) \
    struct STRUCT_NAME \
	{\
	    MAKE_ATT(ATT) \
	};\
    std::ostream& operator<<(std::ostream& os, STRUCT_NAME const&obj) \
    {\
        return os << BOOST_PP_STRINGIZE(STRUCT_NAME) << "[" << STREAM_ATT_VALUES(ATT) << "]"; \
    }\
	BOOST_DESCRIBE_STRUCT(STRUCT_NAME, (), (GET_ATT_NAMES(ATT)))

