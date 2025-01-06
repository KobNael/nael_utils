#include "model_streamable.hh"

std::ostream &operator<<(std::ostream &os, details::streamable const &obj)
{
    return obj.stream(os);
}
