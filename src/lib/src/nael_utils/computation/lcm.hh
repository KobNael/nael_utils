/**
 *@file lcm.hh
 */
#pragma once
#include <set>

/**
 * @brief Compute the lowest common multiple of a list of integer values
 * @param values the list of values
 * @return the lowest common multiple of a list of integer values
 * @pre values is not empty
 * @pre values does not contain 0
 */
unsigned long compute_lcm(const std::set<unsigned, std::less<>>& values);
