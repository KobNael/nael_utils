#pragma once

#include "utils/model_desc.hh"

#include "Press.hh"
#include "Mold.hh"
#include <unordered_map>


namespace bo
{

class BoContext
{
public:
    virtual ~BoContext();
//          (std::vector<Product>, products) )

    MAKE_UNORDERED_MAP(std::string, Press, presses)
    MAKE_MAP(std::string, Mold, molds)
//Basic attributes
//    MAKE_CLASS_ATT(
//        //Const attributes
//        ,
//        //Editable attributes
//        ( (std::vector<Press>)(press) )
//    )
};


}