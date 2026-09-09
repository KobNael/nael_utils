
/**
 *@file lcm.hh
 */

#include <nael_utils/computation/lcm.hh>

#include <nael_utils/exception/exception.hh>

#include <numeric>   // std::lcm

// Compute the lowest common multiple of a list of integer values
unsigned long compute_lcm(const std::set<unsigned, std::less<>>& values)
{
    CHECK_PRE(!values.empty(), "compute_lcm : vector can not be empty.")

    unsigned long result = 1;
    for (unsigned v : values) {
        CHECK_PRE(v != 0, "compute_lcm : invalid value O.")
        unsigned long abs_v = static_cast<unsigned long>(v);
        result = std::lcm(result, abs_v);
    }
    return result;
}
