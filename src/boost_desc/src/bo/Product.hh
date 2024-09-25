#pragma once

#include "utils/model_desc.hh"

#include<vector>

namespace bo
{
/**
    CLASS Product
        STRING ref CONST
        //Production
        MAP POINTER Mold molds CONST_ITEMS
        UNSIGNED INT minLotSize NO_INIT
        DURATION productionTimeByUnit NO_INIT VALUE "boost::posix_time::seconds(2)"
        MAP POINTER Configuration configurations NO_INIT CONST_ITEMS
        //Storage
        UNSIGNED INT capacity NO_INIT
        FLOAT maxUsage NO_INIT VALUE "1."
        FLOAT maxUsageIfRelax NO_INIT VALUE "1.4"
        //Safety stock
        UNSIGNED INT initialStock NO_INIT
        UNSIGNED INT minSafetyStock NO_INIT
        UNSIGNED INT maxSafetyStock NO_INIT
        UNSIGNED INT curSafetyStock NO_INIT
        //Demands
        SET POINTER Demand demands CONST_ITEMS
    END CLASS
 */
class Product
{

    MAKE_CLASS_ATT(
        ( ( std::string, id )
          ( unsigned, minLotSize )
          ( unsigned, initialStock )
          ( unsigned, capacity )
          ( float, maxUsage )
          ( float, maxUsageIfRelax )
          ( boost::posix_time::time_duration, productionTime ) ),
        ( ( std::set<Demand>, demands )
          ( std::vector<Mold>, molds )
          ( unsigned, safety ) )
        )
}

}
