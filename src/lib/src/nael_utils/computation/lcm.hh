/**
 *@file lcm.hh
 *@brief Compute the lowest common mutlipe of a list of integer values
 */
#pragma once
#include <vector>

/**
 * @brief Compute the lowest common multiple of two integer values
 * @param a the first value
 * @param b the second value
 * @return the lowest common multiple of a and b
 */
unsigned long compute_lcm(unsigned long a, unsigned long b);

/**
 * @brief Compute the lowest common multiple of a list of integer values
 * @param values the list of values
 * @return the lowest common multiple of a list of integer values
 */
unsigned long compute_lcm(const std::vector<unsigned>& values);
