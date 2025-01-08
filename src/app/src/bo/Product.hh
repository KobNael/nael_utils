#pragma once

#include "Demand.hh"
#include "Mold.hh"

#include <nael_utils/model/model_desc.hh>

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
        ( ( boost::posix_time::time_duration)(productionTime)(boost::posix_time::seconds(0)) ),
        ( ( std::set<Demand>)(demands) )
        ( ( std::vector<Mold*>)(molds) )
        ( ( unsigned)(safety) )
    )
};

}
