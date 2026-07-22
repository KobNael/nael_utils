#pragma once

#include "test_data_model.hh"

#include <string>

namespace dto
{

    /**
     * @brief Create and return a BasicAttDto object
     * @param id the value of the string attribute
     */
    BasicAttDto makeBasicAttDto(std::string const &id="id");

    /**
     * @brief Create and return an IdDto object
     * @param id the value of the string attribute
     */
    IdDto makeIdDto(std::string const &id="id");

    /**
     * @brief Create and return an ParamDto object
     * @param param the enum value corresponding to the parameter
     * @param val the value
     */
    ParamDto makeParamDto(dto::EnumDto param, std::string const &val);

} //namespace dto
