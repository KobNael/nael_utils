#pragma once

#include <nael_utils/model/model_desc.hh>
#include "Closing.hh"
#include "Press.hh"

#include<vector>
#include<set>

namespace bo
{

class Mold
{
//Basic attributes
    MAKE_CLASS_ATT(
        //Non editable att
        ( ( std::string)(id ) ),
        //Editable att
        ( (bool)(allowOffloading ) )
        ( (float)(craneUsageOnSetUp) )
        ( (boost::posix_time::time_duration)(setUp) )
        ( (boost::posix_time::time_duration)(setDown) )
        ( (boost::posix_time::time_duration)(idleTimeBeforeChangeOver) )
        ( (boost::posix_time::time_duration)(idleTimeAfterChangeOver) )
        ( (std::set<MoldClosing>)(mold_closing) )
        ( (std::vector<Press const*>)(presses) )
        ( (std::vector<std::string>)(configurations) )
    )

};

}
