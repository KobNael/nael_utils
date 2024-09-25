#include "model_desc_print.hh"

std::ostream &operator<<(std::ostream &os, details::streamable const &obj)
{
    return obj.stream(os);
}
