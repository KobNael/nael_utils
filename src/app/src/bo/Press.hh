#pragma once

#include <nael_utils/model/model_desc.hh>
#include "Closing.hh"

#include<vector>
#include<set>

namespace bo
{

class Press
{
//Basic attributes
    MAKE_CLASS_ATT(
        ( (std::string)(id) ),
        ( (std::set<PressClosing>)(press_closing ) )
        ( (std::set<ChangeOverClosing>)(co_closing ) )
    )
};

}
