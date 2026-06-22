/**
 * @file nael_utils/containers/sorted_list.hh
 * @brief Utilities for sorted lists
 */
#pragma once

#include <list>
#include <algorithm>
#include <ranges>

/**
 * @brief Add an element in a sorted list
 * @param to_add the element to add
 * @param list the list of values
 */
template<typename T>
void add_element_in_sorted_list(T const &to_add, std::list<T> &list)
{
    //search last item smaller than to_add
    auto it = std::ranges::find_if(list | std::views::reverse,
                            [to_add](auto const &value)
                            { return value <= to_add; }).base();
    // insert the new value
    list.insert(it, to_add);
}
/**
 * @brief Remove the first occurence of a duration from a vector of durations
 * @param to_remove the duration to remove
 * @param durations the list of durations
 */
template<typename T>
void remove_element_from_sorted_list(T const &to_remove, std::list<T> &list)
{
    auto it = std::ranges::find(list, to_remove);
    // check that there actually is a to_remove in our list
    if (it != list.end()) {
        list.erase(it);
    }
}

/**
 * @brief Check if two sorted lists intersect
 * @param list1 the first list
 * @param list2 the second list
 * @return true if the two lists intersect, false otherwise
 */
template<typename T>
bool intersect(std::list<T> const &list1, std::list<T> const &list2)
{
    auto it1 = list1.begin();
    auto it2 = list2.begin();

    while (it1 != list1.end() && it2 != list2.end()) {
        if (*it1 == *it2) {
            return true;
        }

        if (*it1 < *it2) {
            ++it1;
        } else {
            ++it2;
        }
    }

    return false;
}