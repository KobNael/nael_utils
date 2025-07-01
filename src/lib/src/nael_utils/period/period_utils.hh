#pragma once

#include <list>
#include <nael_utils/period/period_typedef.hh>

/**
 * @file period_utils.hh
 * @brief Toolbox for manipulation of time period.   
 * Two types of time period are proposed, along with shortcuts for list of periods:
 * - #time_period : shortcut for boost::posix_time::time_period
 * - capa_period : #time_period with capacity (long)
 * - ratio_period : #time_period with a ratio (float)
 *
 * One can then compute the union, the intersection or the difference between list of periods:
 *  - Union of capa_periods : compute the sum of the capacity
 *    - the union of {[a, b, 2]} and {[b, c, 3]}, with a<b<c is {[a, b, 2], [b, c, 3]}
 *    - the union of {[a, b, 2]} and {[a, b, 3]} is {[a, b, 5]}
 *    - the union of {[a, c, 2]} and {[a, b, 3]}, with a<b<c, is {[a, b, 5], [b, c, 2]}
 *  - Union of ratio_periods : compute the product of the ratios (the default value being 1.)
 *    - the union of {[a, b, 2.]} and {[a, b, 3.]} is {[a, b, 5.]}
 *    - the union of {[a, c, 2.]} and {[a, b, 3.]}, with a<b<c, is {[a, b, 6.], [b, c, 3.]}
 *  - Intersection of capa_periods : limit to the intersection with minimal capacity
 *    - the intersection of {[a, b, 2]} and {[a, b, 3]} is {[a, b, 2]}
 *    - the intersection of {[a, c, 2]} and {[a, b, 3]}, with a<b<c, is {[a, b, 2]}
 *  - Intersection of ratio_periods and periods : limit the ratio_periods to the other periods
 *    - the intersection of {[a, c, 2.]} and {[a, b]}, with a<b<c, is {[a, b, 2.]}
 *  - Difference of capa_periods : compute the difference of the overlapping capacities
 *    - the difference of {[a, b, 2]} and {[b, c, 3]}, with a<b<c is {[a, b, 2], [b, c, -3]}
 *    - the difference of {[a, b, 2]} and {[a, b, 3]} is {[a, b, -1]}
 *    - the difference of {[a, c, 2]} and {[a, b, 3]}, with a<b<c, is {[a, b, -1], [b, c, 2]}
 *
 * @remark Each method has an optional parameter merge_adjacent telling if one should merge the adjacent periods (with the same capacity or ratio)
 * @remark Every capa_period with a capacity of 0 will be removed
 * @remark In some case, one may need to keep 'empty' periods (eg. to compute some ending times)
 * @warning Every list must contain sorted and disjoint (or adjacent) periods
 */

///////////
// Union //
///////////
/**
 * @brief Compute the union of two lists of Periods
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param periods1 first list
 * @param periods2 second list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the lists must be sorted and contain disjoint periods
 * @return the union
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_union(std::list<PeriodT> const &periods1, std::list<PeriodU> const &periods2, bool merge_adjacent=true);
/**
 * @brief Compute the union of a period and a list of periods
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param period the period
 * @param periods the list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the union
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_union(PeriodT const &period, std::list<PeriodU> const& periods, bool merge_adjacent=true)
{
    return get_union(std::list<PeriodT>(1,period), periods, merge_adjacent);
}

/**
 * @brief Compute the union of a list of periods and a period
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param period the period
 * @param periods the list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the union
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_union(std::list<PeriodT> const &periods, PeriodU const &period, bool merge_adjacent=true)
{
    return get_union(periods, std::list<PeriodU>(1,period), merge_adjacent);
}
/**
 * @brief Compute the union of two periods
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param period1 the first period
 * @param period2 the second period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the union
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_union(PeriodT const &period1, PeriodU const& period2, bool merge_adjacent=true)
{
    return get_union(std::list<PeriodT>(1,period1), std::list<PeriodU>(1, period2), merge_adjacent);
}

////////////////
// Difference //
////////////////
/**
 * @brief Compute the difference of two lists of Periods
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param periods1 first list
 * @param periods2 second list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the lists must be sorted and contain disjoint periods
 * @return the difference
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_diff(std::list<PeriodT> const &periods1, std::list<PeriodU> const &periods2, bool merge_adjacent=true);
/**
 * @brief Compute the difference of a period and a list of periods
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param period the period
 * @param periods the list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the difference
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_diff(PeriodT const &period, std::list<PeriodU> const& periods, bool merge_adjacent=true)
{
    return get_diff(std::list<PeriodT>(1,period), periods, merge_adjacent);
}
/**
 * @brief Compute the difference of a list of periods and a period
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param periods the list
 * @param period the period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the difference
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_diff(std::list<PeriodT> const &periods, PeriodU const &period, bool merge_adjacent=true)
{
    return get_diff(periods, std::list<PeriodU>(1,period), merge_adjacent);
}
/**
 * @brief Compute the difference of two periods
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param period1 the first period
 * @param period2 the second period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the difference
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_diff(PeriodT const &period1, PeriodU const& period2, bool merge_adjacent=true)
{
    return get_diff(std::list<PeriodT>(1,period1), std::list<PeriodU>(1, period2), merge_adjacent);
}

//////////////////
// Intersection //
//////////////////
/**
 * @brief Compute the intersection of two lists of Periods
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param periods1 first list
 * @param periods2 second list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the lists must be sorted and contain disjoint periods
 * @return the intersection
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter(std::list<PeriodT> const &periods1, std::list<PeriodU> const &periods2, bool merge_adjacent=true);
/**
 * @brief Compute the intersection of a period and a list of periods
 * @tparam PeriodT the type of first period
 * @tparam PeriodU the type of second periods
 * @param period the period
 * @param periods the list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the intersection
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter(PeriodT const &period, std::list<PeriodU> const& periods, bool merge_adjacent=true)
{
    return get_inter(std::list<PeriodT>(1,period), periods, merge_adjacent);
}
/**
 * @brief Compute the intersection of a list of periods and a period
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second period
 * @param periods the list
 * @param period the period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the intersection
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter(std::list<PeriodT> const &periods, PeriodU const &period, bool merge_adjacent=true)
{
    return get_inter(periods, std::list<PeriodU>(1,period), merge_adjacent);
}
/**
 * @brief Compute the intersection of two period
 * @tparam PeriodT the type of first period
 * @tparam PeriodU the type of second period
 * @param period1 the first period
 * @param period2 the second period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the intersection
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter(PeriodT const &period1, PeriodU const& period2, bool merge_adjacent=true)
{
    return get_inter(std::list<PeriodT>(1,period1), std::list<PeriodU>(1, period2), merge_adjacent);
}
/**
 * @brief Compute the intersection of two lists of Periods and keep the periods with just one time stamp
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
 * @param periods1 first list
 * @param periods2 second list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the lists must be sorted and contain disjoint periods
 * @return the intersection
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter_with_empty(std::list<PeriodT> const &periods1, std::list<PeriodU> const &periods2, bool merge_adjacent=true);
/**
 * @brief Compute the intersection of two lists of Periods and keep the periods with just one time stamp
 * @tparam PeriodT the type of first period
 * @tparam PeriodU the type of second periods
 * @param period the period
 * @param periods the list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the intersection
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter_with_empty(PeriodT const &period, std::list<PeriodU> const& periods, bool merge_adjacent=true)
{
    return get_inter_with_empty(std::list<PeriodT>(1,period), periods, merge_adjacent);
}
/**
 * @brief Compute the intersection of two lists of Periods and keep the periods with just one time stamp
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second period
 * @param periods the list
 * @param period the period
 * @param merge_adjacent should we merge the adjacent periods
 * @param keep_empty should we keep the empty periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the intersection
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter_with_empty(std::list<PeriodT> const &periods, PeriodU const &period, bool merge_adjacent=true)
{
    return get_inter_with_empty(periods, std::list<PeriodU>(1,period), merge_adjacent);
}
/**
 * @brief Compute the intersection of two lists of Periods and keep the periods with just one time stamp
 * @tparam PeriodT the type of first period
 * @tparam PeriodU the type of second period
 * @param period1 the first period
 * @param period2 the second period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the intersection
 */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter_with_empty(PeriodT const &period1, PeriodU const& period2, bool merge_adjacent=true)
{
    return get_inter_with_empty(std::list<PeriodT>(1,period1), std::list<PeriodU>(1, period2), merge_adjacent);
}
/**
 * @brief Compute a relative duration
 * @param duration the absolute duration
 * @param ratio the ratio to apply
 * @return the relative duration
 */
boost::posix_time::time_duration compute_relative_duration(boost::posix_time::time_duration duration, float ratio);

/**
 * @brief Compute an absolute duration
 * @param duration the relative duration
 * @param ratio the ratio to apply
 * @return the absolute duration
 */
boost::posix_time::time_duration compute_theoretical_duration(boost::posix_time::time_duration duration, float ratio);

/**
 * @brief Compute the cumulative duration of a list of periods
 * @tparam PeriodT the type of periods
 * @param periods the periods of production
 * @return the cumulative duration
 */
template<typename PeriodT>
boost::posix_time::time_duration get_total_duration(std::list<PeriodT> const &periods);

/**
 * @brief Compute the relative duration of a list of periods
 * @param periods the periods with ratio
 * @return the relative duration
 */
boost::posix_time::time_duration get_relative_duration(LRatioPeriod const &periods);

/**
 * @brief Collect the capa_period with enough capacity and return them as time_period
 * @param periods the list of capa_periods
 * @param min the minimal capacity
 * @return the corresponding time_periods
 */
LTimePeriod get_eligible_periods(LCapaPeriod const &periods, unsigned min);

/**
 * @brief Create an artificial empty period
 * @param datetime the ptime
 * @return a time period [ptime, ptime+10 milliseconds[
 */
time_period make_empty_period(boost::posix_time::ptime ptime);

/**
 * @brief Checks that two ptime are the same (with a tolerance)
 * @param lhs the first datetime
 * @param rhs the second datetime
 * @param tol the tolerance
 */
bool is_same(boost::posix_time::ptime const &lhs, boost::posix_time::ptime const &rhs, boost::posix_time::time_duration const &tol=boost::posix_time::milliseconds(10));

//Implementation
#include <nael_utils/period/period_utils.tpp>
