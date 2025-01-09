#pragma once
#include <string>

namespace model_test
{
    struct BasicAttDto;

    /**
     * @brief Create and return a BasicAttDto object
     * @param id_p the value of the string attribute
     */
    BasicAttDto makeBasicAttDto(std::string const &id_p="id");

} //namespace model_test
