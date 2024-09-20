#include "model_desc_print.hh"

namespace details
{

std::ostream &operator<<(std::ostream &os, streamable const &obj)
{
    return obj.stream(os);
}

}//details

