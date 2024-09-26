#pragma once

#include "utils/model_desc.hh"
#include "Closing.hh"

#include<vector>
#include<set>

namespace bo
{

class Press
{
    MAKE_CLASS_ATT(
        ( std::string, id),
        ( std::set<PressClosing>, press_closing )
        ( std::set<ChangeOverClosing>, co_closing ) )
};

}
