/** @file model_streamable.cc */

#include <nael_utils/model/details/model_streamable.hh>

namespace details
{
std::ostream &operator<<(std::ostream &os, streamable const &obj)
{
    return obj.stream(os);
}
}