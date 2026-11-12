#include <nael_utils/period/period_typedef.hh>

namespace bpt = boost::posix_time;

extended_period::~extended_period() = default;

extern boost::posix_time::time_duration compute_relative_duration(boost::posix_time::time_duration duration, float ratio);

bpt::time_duration ratio_period::get_relative_duration() const
{
    return compute_relative_duration(length(), _ratio);
}
