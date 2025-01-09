#pragma once
#include <string>

namespace model_test
{
    struct BasicAttDto;
    struct IdDto;

    /**
     * @brief Create and return a BasicAttDto object
     * @param id_p the value of the string attribute
     */
    BasicAttDto makeBasicAttDto(std::string const &id_p="id");

    /**
     * @brief Create and return an IdDto object
     * @param id_p the value of the string attribute
     */
    IdDto makeIdDto(std::string const &id_p="id");

} //namespace model_test
