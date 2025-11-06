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

