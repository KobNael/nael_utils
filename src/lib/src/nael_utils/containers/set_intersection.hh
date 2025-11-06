/**
 * @file nael_utils/containers/set_intersection.hh
 * @brief Utilities for set intersection
 */
#pragma once

#include <set>

/**
 * @brief check for sets intersection
 * @return true if two sets intersects
 * @tparam T the type of the elements of the sets
 */
template<typename T, typename Compare = std::less<>>
bool intersect(std::set<T> const &set1, std::set<T> const &set2)
{
    auto it1 = set1.begin();
    auto it2 = set2.begin();
    while (it1 != set1.end() && it2 != set2.end())
    {
        if (Compare{}(*it1, *it2))
        {
            ++it1;
        }
        else if (Compare{}(*it2, *it1))
        {
            ++it2;
        }
        else
        {
            return true;
        }
    }
    return false;
}
