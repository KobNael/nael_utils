#include <nael_utils/model/details/model_streamable.hh>
/**
 * @brief Create a new type of numerical identifier
 * @param class_name the name of the class
 * @warning avoid to use this tool inside a namespace
 *
 * Example:
 * @code{cpp}
 *     MAKE_IDX(ObjIdx)
 * @endcode
 *   expands to
 * @code{cpp}
 * struct ObjIdx
 * {
 *  public:
 *      size_t _val;
 *      explicit ObjIdx(): _val(std::numeric_limits<size_t>::max()) {}
 *      explicit ObjIdx(const size_t &val_p): _val(val_p) {}
 *      bool is_unsetted() const {return _val == class_name()._val;
 * }
 * @endcode
 */
#define MAKE_IDX(class_name)                                                   \
    struct class_name : public details::streamable                             \
    {                                                                          \
    public:                                                                    \
        size_t _val;                                                           \
        explicit class_name() : _val(std::numeric_limits<size_t>::max()) {}    \
        explicit class_name(const size_t &val_p) : _val(val_p) {}              \
        bool is_unsetted() const { return _val == class_name()._val; }          \
        std::ostream &stream(std::ostream &os) const override                  \
        {                                                                      \
            return os << BOOST_PP_STRINGIZE(class_name) << "(" << _val << ")"; \
        }                                                                      \
        bool operator==(class_name const &) const = default;                   \
        bool operator<(class_name const &other) const                          \
        {                                                                      \
            return _val < other._val;                                          \
        }                                                                      \
    };
