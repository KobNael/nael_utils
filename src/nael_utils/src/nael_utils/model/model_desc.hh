/** @file model_desc.hh */

#pragma once

#include "details/model_streamable.hh"
#include "details/model_desc_print.hh"
#include "details/model_desc_att.hh"
#include "nael_utils/safe_comp/safe_comp.hh"

#include <boost/describe.hpp>


/**
 * @brief Create an operator< based on a list of attributes.<br/>
 * @code{cpp}
 *     MAKE_CLASS_SORT(AbstractClosing, (from, to) )
 * @endcode
 *  expands to
 * @code{cpp}
 *    public: bool operator<(AbstractClosing const&other) const
 *     {
 *       if( safecomp::neq( get_from(), other.get_from() ) )
 *       {
 *          if( safecomp::lt( get_from(), other.get_from() ) )
 *          {
 *             return true;
 *          }
 *          else
 *          {
 *             return false;
 *          }
 *       }
 *       if( safecomp::neq( get_to(), other.get_to() ) )
 *       {
 *          if( safecomp::lt( get_to() , other.get_to() ) )
 *          {
 *             return true;
 *          }
 *          else
 *          {
 *             return false;
 *          }
 *       }
 *       return false;
 *    }
 * @endcode
 * @param class_name the name of the class
 * @param att_list the list of attributes to sort on
 */
#define MAKE_CLASS_SORT(class_name, att_list) \
public: \
    bool operator<(class_name const&other) const\
    {\
        COMPARE_ATT_LIST(BOOST_PP_TUPLE_TO_SEQ(att_list))\
        return false;\
    }


/**
 * @brief allow the declaration of a structure
 * @code{cpp}
 *   MAKE_DTO_STRUCT(
 *       PressDto,
 *       ( (std::string)(id) )
 *       ( (unsigned)(pos) )
 *   )
 * @endcode
 *   expands to
 * @code{cpp}
 *     struct PressDto
 *     {
 *         std::string id;
 *         unsigned pos;
 *     };
 * @endcode
 * @param struct_name the name of the struct
 * @param att_seq the sequence of sequences describing the attribute
 * @remark these structures are serializable and streamable
 * @see  json::import_from_file() and json::export_to_file()
 * @see  details::streamable
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
        bool operator==(struct_name const&) const = default;\
    };\
    BOOST_DESCRIBE_STRUCT(struct_name, (), (GET_ATT_NAMES(att_seq)))

/**
 * @brief allow the declaration of an enum
 * @code{cpp}
 *  MAKE_DTO_ENUM(
 *      //Name
 *      EnumDto,
 *      //Values
 *      VAL_1,
 *      VAL_2,
 *      VAL_3
 *  )
 * @endcode
 * @remark it is just an alias for BOOST_DEFINE_ENUM
 * @remark these structures are serializable and streamable
 * @see  json::import_from_file() and json::export_to_file()
 */
#define MAKE_DTO_ENUM BOOST_DEFINE_ENUM

/**
 * @brief Generate an editable attribute std::map<key, value> name along with its getters
 * @code{cpp}
 *     MAKE_MAP(std::string, int, values)
 * @endcode
 *  expands to
 * @code{cpp}
 *    private:
 *     std::map<std::string, int> values;
 *    public:
 *     std::map<std::string, int> &get_values()
 *     {
 *       return values;
 *     }
 *     std::map<std::string, int> const &get_values() const
 *     {
 *       return values;
 *     }
 * @endcode
 * @param key the type of keys
 * @param value the type of values
 * @param name the name of the attribute
 */
#define MAKE_MAP(key, value, name) MAKE_MAP_ATT(key, value, name, 0)
/**
 * @brief Generate an editable attribute std::unordered_map<key, value> name along with its getters
 * @code{cpp}
 *     MAKE_UNORDERED_MAP(std::string, int, values)
 * @endcode
 *  expands to
 * @code{cpp}
 *    private:
 *     std::unordered_map<std::string, int> values;
 *    public:
 *     std::unordered_map<std::string, int> &get_values()
 *     {
 *       return values;
 *     }
 *     std::unordered_map<std::string, int> const &get_values() const
 *     {
 *       return values;
 *     }
 * @endcode
 * @param key the type of keys
 * @param value the type of values
 * @param name the name of the attribute
 */
#define MAKE_UNORDERED_MAP(key, value, name) MAKE_MAP_ATT(key, value, name, 1)

/**
 * @brief Generate an editable attribute std::set<value> name along with its getters
 * @code{cpp}
 *     MAKE_SET(int, values)
 * @endcode
 *  expands to
 * @code{cpp}
 *    private:
 *     std::set<int> values;
 *    public:
 *     std::set<int> &get_values()
 *     {
 *       return values;
 *     }
 *     std::set<int> const &get_values() const
 *     {
 *       return values;
 *     }
 * @endcode
 * @param value the type of values
 * @param name the name of the attribute
 */
#define MAKE_SET(value, name) MAKE_SET_ATT(value, name, 0)
/**
 * @brief Generate an editable attribute std::unordered_set<value> name along with its getters
 * @code{cpp}
 *     MAKE_UNORDERED_SET(int, values)
 * @endcode
 *  expands to
 * @code{cpp}
 *    private:
 *     std::unordered_set<int> values;
 *    public:
 *     std::unordered_set<int> &get_values()
 *     {
 *       return values;
 *     }
 *     std::unordered_set<int> const &get_values() const
 *     {
 *       return values;
 *     }
 * @endcode
 * @param value the type of values
 * @param name the name of the attribute
 */
#define MAKE_UNORDERED_SET(value, name) MAKE_SET_ATT(value, name, 1)

/**
 * @brief Generate every set / get for basic attributes
 * @code{cpp}
 *     MAKE_CLASS_ATT(
 *         //non editable attributes
 *         ( (std::string)(id) ),
 *         //editable attributes
 *         ( (unsigned)(x) )
 *         ( (unsigned)(pos) )
 *     )
 * @endcode
 *   expands to
 * @code{cpp}
 *   private:
 *       std::string id;
 *   public:
 *       std::string const &get_id() const { return id; }
 *   private:
 *       unsigned x;
 *   public:
 *       void set_x(unsigned p) { x = p; }
 *       unsigned &get_x() { return x; }
 *       unsigned get_x() const { return x; }
 *   private:
 *       unsigned pos;
 *   public:
 *       void set_pos(unsigned p) { pos = p; }
 *       unsigned &get_pos() { return pos; }
 *       unsigned get_pos() const { return pos; }
 * @endcode
 * @param const_att_seq sequence of non editable attributes
 * @param editable_att_seq sequence of editable attributes
 */
#define MAKE_CLASS_ATT( const_att_seq, editable_att_seq ) \
    MAKE_CLASS_BASIC_ATT( const_att_seq, 1 ) \
    MAKE_CLASS_BASIC_ATT( editable_att_seq, 0 )

