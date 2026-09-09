
/**
 *@file lcm.hh
 *@brief Compute the lowest common mutlipe of a list of integer values
 */

#include <nael_utils/computation/lcm.hh>

#include <nael_utils/exception/exception.hh>

#include <numeric>   // std::gcd

// Compute the lowest common multiple of two integer values
unsigned long compute_lcm(unsigned long a, unsigned long b)
{
    CHECK_PRE(a != 0 && b != 0, "compute_lcm : invalid value 0.")

    // divide before to avoid overflow
    return (a / std::gcd(a, b)) * b;
}

// Compute the lowest common multiple of a list of integer values
unsigned long compute_lcm(const std::vector<unsigned>& values)
{
    CHECK_PRE(!values.empty(), "compute_lcm : vector can not be empty.")

    unsigned long result = 1;
    for (unsigned v : values) {
        unsigned long abs_v = static_cast<unsigned long>(v);
        result = compute_lcm(result, abs_v);
    }
    return result;
}
