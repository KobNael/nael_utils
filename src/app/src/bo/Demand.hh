#pragma once

#include <nael_utils/model/model_desc.hh>

#include<vector>

namespace bo
{
class Demand
{
//Basic attributes
    MAKE_CLASS_ATT(
        ,
        ( ( boost::gregorian::date)(date)(boost::gregorian::date(boost::gregorian::not_a_date_time)) )
        ( ( unsigned)(demand)(0) )
        ( ( unsigned)(safety)(0) )
    )
};

}
