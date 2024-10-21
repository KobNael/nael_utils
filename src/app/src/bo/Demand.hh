#pragma once

#include "utils/model_desc.hh"

#include<vector>

namespace bo
{
class Demand
{
//Basic attributes
    MAKE_CLASS_ATT(
        ( ( boost::gregorian::date)(date) )
        ( ( unsigned)(demand) )
        ( ( unsigned)(safety) ),
        ( ( boost::gregorian::date)(date) )
        ( ( unsigned)(demand ) )
        ( ( unsigned)(safety ) )
    )
}

}
