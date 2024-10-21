#pragma once

#include "utils/model_desc.hh"

#include<vector>

namespace bo
{
class Product
{

//Basic attributes
    MAKE_CLASS_ATT(
        ( ( std::string)(id) )
        ( ( unsigned)(minLotSize) )
        ( ( unsigned)(initialStock) )
        ( ( unsigned)(capacity) )
        ( ( float)(maxUsage) )
        ( ( float)(maxUsageIfRelax) )
        ( ( boost::posix_time::time_duration)(productionTime) ),
        ( ( std::set<Demand>)(demands) )
        ( ( std::vector<Mold>)(molds) )
        ( ( unsigned)(safety) )
    )
}

}
