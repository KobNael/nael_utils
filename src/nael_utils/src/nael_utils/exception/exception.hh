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

    virtual const char* what() const noexcept override
    {
        return _message.c_str();
    }

    virtual ~NaelException() = 0;

private:
    std::string _message;
};

/**
 * @class ConsistencyException
 * @brief Pure virtual class to create dedicated exceptions
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

