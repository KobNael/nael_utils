#pragma once

#include <boost/preprocessor.hpp>
#include <boost/describe.hpp>
namespace details
{
template<typename T>
std::ostream &print(std::ostream &os, T const &obj)
{
    return os << obj;
}
template<typename T>
std::ostream &print(std::ostream &os, std::vector<T> const &obj)
{
    os << "[";
    bool first{true};
    for(T const &e : obj)
    {
        os << ((first)?"":",") << e;
        first = false;
    }
    return os << "]";
}
}//details
#define STREAM_ATT_VALUE_VARIABLE(r, data, i, elem) \
	details::print( os << BOOST_PP_IF(i, ", " <<,) BOOST_PP_STRINGIZE( BOOST_PP_TUPLE_ELEM(1, elem) ) << "=" BOOST_PP_COMMA() \
    obj.BOOST_PP_TUPLE_ELEM(1, elem) );

#define STREAM_ATT_VALUES(seq) \
    BOOST_PP_SEQ_FOR_EACH_I(STREAM_ATT_VALUE_VARIABLE, , BOOST_PP_VARIADIC_SEQ_TO_SEQ(seq))

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
        os << BOOST_PP_STRINGIZE(STRUCT_NAME) << "{";\
        STREAM_ATT_VALUES(ATT)\
        return os << "}"; \
    }\
	BOOST_DESCRIBE_STRUCT(STRUCT_NAME, (), (GET_ATT_NAMES(ATT)))

