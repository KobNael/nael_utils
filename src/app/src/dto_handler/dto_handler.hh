#pragma once
#include "dto/In.hh"
#include "bo/In.hh"

namespace dto_handler
{

    void dto_to_bo(dto::DtoContext const& dto_context , bo::BoContext &bo_context);

}
