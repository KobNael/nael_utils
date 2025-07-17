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
    public:
        /**
         * @brief Print the structure description in a stream
         * @param os the stream
         */
        virtual std::ostream &stream(std::ostream &os) const = 0;

        /**
         * @brief Default Comparison operator
         */
        bool operator==(streamable const &) const = default;

    private:
        /** @brief OStream operator for streamable structure */
        friend std::ostream &operator<<(std::ostream &os, streamable const &obj)
        {
            return obj.stream(os);
        }
    };

} // details
