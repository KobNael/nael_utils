#pragma once

#include "utils/model_desc.hh"

#include "Press.hh"

namespace bo
{

class BoContext
{
public:
    virtual ~BoContext();
//          (std::vector<Product>, products) )
    MAKE_CLASS_ATT(
        //Const attributes
        ,
        //Editable attributes
        (std::vector<std::string>, press)
        (std::vector<std::string>, mold) )

};


}