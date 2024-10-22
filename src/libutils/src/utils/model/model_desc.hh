#pragma once

#include "details/model_desc_print.hh"
#include "details/model_desc_att.hh"
#include "utils/safe_comp/safe_comp.hh"

#include <boost/describe.hpp>

/**
  @brief Create an operator< based on a list of attributes
    MAKE_CLASS_SORT(AbstractClosing, (from, to) )
  expands to
    public: bool operator<(AbstractClosing const&other) const
    {
      if( safecomp::neq( get_from(), other.get_from() ) )
      {
         if( safecomp::lt( get_from(), other.get_from() ) )
         {
            return true;
         }
         else
         {
            return false;
         }
      }
      if( safecomp::neq( get_to(), other.get_to() ) )
      {
         if( safecomp::lt( get_to() , other.get_to() ) )
         {
            return true;
         }
         else
         {
            return false;
         }
      }
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
        ( (std::string)(id) )
        ( (unsigned)(pos) )
    )
  expands to
    struct PressDto
    {
        std::string id;
        unsigned pos;
    };
  @remark these structures are serializable (see json::import_from_file)
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
 @brief Generate an editable attribute std::unordered_map<key, value> name
 along with its getters
 */
#define MAKE_MAP(key, value, name) MAKE_MAP_ATT(key, value, name, 0)
/**
 @brief Generate an editable attribute std::map<key, value> name
 along with its getters
 */
#define MAKE_UNORDERED_MAP(key, value, name) MAKE_MAP_ATT(key, value, name, 1)

/**
 @brief Generate an editable attribute std::set<value> name
 along with its getters
 */
#define MAKE_SET(value, name) MAKE_SET_ATT(value, name, 0)
/**
 @brief Generate an editable attribute std::unordered_set<value> name
 along with its getters
 */
#define MAKE_UNORDERED_SET(value, name) MAKE_SET_ATT(value, name, 1)

/**
  @brief Generate every set / get for basic attributes
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
