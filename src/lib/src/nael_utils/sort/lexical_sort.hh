/**
 * @file lexical_sort.hh
 * @brief Interface for lexical sort with tolerance
 */
#pragma once

#include <algorithm>

/**
 * @brief Interface for lexical comparator
 *
 * Must implement two functions :
 *      - is_better : strict comparison (without tolerance)
 *      return true if left hand side is better than right hand side
 *      - is_equivalent : comparison with tolerance
 *      return true if both sides are equivalent
 * @tparam T type of elements to compare
 */
template <typename T>
struct LexicalComparator
{
    /**
     * @brief Destructor
     */
    virtual ~LexicalComparator() = default;
    /**
     * @brief Strict comparison
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true if lhs is strictly better than rhs
     */
    virtual bool is_better(T const &lhs, T const &rhs) const = 0;
    /**
     * @brief Comparison with tolerance
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true if lhs and rhs are equivalent
     */
    virtual bool is_equivalent(T const &lhs, T const &rhs) const = 0;
};

/**
 * @struct Comparator
 * @brief Comparator for strict comparison
 * @tparam T the type of elements to compare
 */
template <typename T>
struct Comparator
{
    /**
     * @brief Constructor
     */
    explicit Comparator<T>(LexicalComparator<T> const *cmp) : internal_cmp(cmp)
    {
    }

    /**
     * @brief Pointer to original LexicalComparator
     */
    LexicalComparator<T> const *internal_cmp = nullptr;

    /**
     * @brief Operator of comparison
     * @param lhs left hand side
     * @param rhs right hand side
     * @return true if lhs is better than rhs
     * @see LexicalComparator::is_better
     */
    bool operator()(T const &lhs, T const &rhs) const
    {
        return internal_cmp->is_better(lhs, rhs);
    }
};

/**
 * @brief Create a Comparator with template deduction
 * @tparam T the type of elements to compare
 * @param lexical_comp The original LexicalComparator
 * @return a Comparator for strict comparison using lexical_comp
 */
template <typename T>
Comparator<T> makeComparator(LexicalComparator<T> const &lexical_comp)
{
    return Comparator<T>(&lexical_comp);
}

/**
 * @brief Sort in lexical order using a list of LexicalComparators
 *
 * Values are sorted in strict mode according to the first comparator before being regrouped by equivalcence group.
 * An equivalence group is defined such that all elements of the group are considered equal by the comparator.
 *
 * Once the equivalence groups have been defined, they are sorted according to the second criterion,
 * before being grouped again into sub-equivalence groups, and so on.
 *
 * @note it_comparator must be an iterator on LexicalComparator<T> (smart) pointers
 * @note it_to_sort must be an iterator on T values.
 * @tparam it_comparator type of iterator for range of LexicalComparator
 * @tparam it_to_sort type of iterator for range of elements to sort
 * @param cur_comp begin iterator for range of LexicalComparator
 * @param end_comp end iterator for range of LexicalComparator
 * @param start begin iterator for range of elements to sort
 * @param end end iterator for range of elements to sort
 */
template <typename it_comparator, typename it_to_sort>
void lexical_sort(
    it_comparator cur_comp,
    it_comparator end_comp,
    it_to_sort start,
    it_to_sort end)
{
    if (cur_comp == end_comp)
    {
        return;
    }
    // Sort without tolerance, on deterministic mode if there is no other criterion
    else
    {
        std::stable_sort(start, end, makeComparator(**cur_comp));
    }
    it_to_sort cur = start;
    // While there is another criterion
    while (cur != end)
    {
        // Search last element equivalent to current one (using tolerance)
        it_to_sort next = cur;
        while (next != end && (*cur_comp)->is_equivalent(*cur, *next))
        {
            ++next;
        }
        // now sort the range (if any) with next criterion
        if (cur != next)
        {
            lexical_sort(std::next(cur_comp), end_comp, cur, next);
        }
        // handle next range if any
        if (next == end)
        {
            return;
        }
        // update current to last equal element
        cur = next;
    }
}
