#pragma once

#include "dto/utils/model_desc.hh"

#include<vector>

namespace dto
{

MAKE_DTO_STRUCT(
	PressDto,
	(std::string, id, identifier)
	(unsigned, pos, relative position)
)

struct DtoIn
{
    std::vector<PressDto> presses;
};

}
