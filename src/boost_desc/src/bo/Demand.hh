#pragma once

#include "utils/model_desc.hh"

#include<vector>

namespace bo
{
/**
        CLASS Demand
                SORT ON date
            POINTER Product product CONST CHAIN ON TARGET demands
            DATETIME date CONST
            UNSIGNED INT demand CONST
            UNSIGNED INT safetyWindow CONST
        END CLASS

 */
class Demand
{

    MAKE_CLASS_ATT(
        ( ( boost::gregorian::date, date )
          ( unsigned, demand )
          ( unsigned, safety ) ),
        ( ( boost::gregorian::date, date )
          ( unsigned, demand )
          ( unsigned, safety ) ),
        )

    )

}

}
