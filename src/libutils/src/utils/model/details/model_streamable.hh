/**
 * @file model_desc_print.hh
 */

#pragma once
#include <ostream>

namespace details
{

/**
 * @struct streamable
 * @brief Pure abstract structure, used as base class for struct declared using #MAKE_DTO_STRUCT
 */
struct streamable
{
    /**
     * @brief Print the structure description in a stream
     * @param os the stream
     */
    virtual std::ostream &stream(std::ostream &os) const = 0;
};


}//details

/** @brief stream operator for streamable structure */
std::ostream &operator<<(std::ostream &os, details::streamable const &obj);

