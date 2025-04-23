/**
 * @brief Create a new type of numerical identifier
 * @param class_name the name of the class
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
 *      bool isUnsetted() const {return _val == class_name()._val;
 * }
 * @endcode
 */
#define MAKE_IDX(class_name) \
struct class_name \
{ \
public: \
    size_t _val; \
    explicit class_name(): _val(std::numeric_limits<size_t>::max()) {} \
    explicit class_name(const size_t &val_p): _val(val_p) {} \
    bool isUnsetted() const {return _val == class_name()._val;} \
};
