#pragma once
#include <exception>
#include <string>

/**
 * @class NaelException
 * @brief Pure virtual class to create dedicated exceptions
 */
class NaelException : public std::exception
{
public:
    explicit NaelException(const std::string& message)
        : _message(message) {}

    const char* what() const noexcept override
    {
        return _message.c_str();
    }

    ~NaelException() override = 0 ;

private:
    std::string _message;
};

/**
 * @brief Create a new type of exception
 * @param class_name the name of the class
 *
 * @code{cpp}
 * namespace test
 * {
 *     MAKE_EXCEPTION(base)
 *     void throw_base()
 *     {
 *         throw test::base("dedicated error");
 *     }
 * }
 * @endcode
 */
#define MAKE_EXCEPTION(class_name) \
class class_name : public NaelException \
{ \
public: \
    class_name(const std::string& message)\
    : NaelException(message)\
    {}\
    ~class_name() = default;\
};\

namespace bo
{
/**
 * @brief Dedicated exception for consistency
 */
MAKE_EXCEPTION(consistency)
}
/**
 * @brief Dedicated exception for consistency
 */
MAKE_EXCEPTION(fatal_error)

