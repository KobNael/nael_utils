#pragma once

#include <list>
#include <boost/date_time/posix_time/posix_time.hpp>

/**
 * @file period_utils.hh
 * @brief Toolbox for manipulation of time period.   
 * Two types of time period are proposed, along with shortcuts for list of periods:
 * - #time_period : shortcut for boost::posix_time::time_period
 * - capa_period : #time_period with capacity (long)
 *
 * On can then compute the union, the intersection or the difference between list of periods:
 *  - Union : compute the some of the capacity
 *    - the union of {[a, b, 2]} and {[b, c, 3]}, with a<b<c is {[a, b, 2], [b, c, 3]}
 *    - the union of {[a, b, 2]} and {[a, b, 3]} is {[a, b, 5]}
 *    - the union of {[a, c, 2]} and {[a, b, 3]}, with a<b<c, is {[a, b, 5], [b, c, 2]}
 *  - Intersection : limit to the intersection with minimal capacity
 *    - the intersection of {[a, b, 2]} and {[a, b, 3]} is {[a, b, 2]}
 *    - the intersection of {[a, c, 2]} and {[a, b, 3]}, with a<b<c, is {[a, b, 2]}
 *  - Difference : compute the difference of the overlapping capacities
 *    - the difference of {[a, b, 2]} and {[b, c, 3]}, with a<b<c is {[a, b, 2], [b, c, -3]}
 *    - the difference of {[a, b, 2]} and {[a, b, 3]} is {[a, b, -1]}
 *    - the difference of {[a, c, 2]} and {[a, b, 3]}, with a<b<c, is {[a, b, -1], [b, c, 2]}
 *
 * @remark Each method has an optional parameter merge_adjacent telling if one should merge the adjacent periods (with the same capacity)
 * @remark Every capa_period with a capacity of 0 will be removed
 * @warning Every list must contain sorted and disjoint periods
 */


///////////
// Types //
///////////
/**
 * @typedef time_period
 * @brief Alias for time period
 * @addtogroup period_type_def
 */
typedef boost::posix_time::time_period time_period;

/**
 * @struct capa_period
 * @brief Represents a capacity on a time_period
 * @addtogroup period_type_def
 */
struct capa_period
{
    /**
     * @brief constructor
     * @param capa the capacity
     * @param period the time_period
     */
    capa_period(long capa, time_period const &period)
        : _capa(capa)
        , _period(period)
    {}
    /**
     * @brief constructor
     * @param capa the capacity
     * @param start the starting date time
     * @param end the ending date time
     */
    capa_period(long capa, boost::posix_time::ptime const &start, boost::posix_time::ptime const &end)
        : _capa(capa)
        , _period(time_period(start, end))
    {}
    /** @return the begin of the period */
    boost::posix_time::ptime begin() const { return _period.begin(); }
    /** @return the end of the period */
    boost::posix_time::ptime end() const { return _period.end(); }
    /** @return true if the time_period intersects another time_period */
    bool intersect(capa_period const &cp) const { return _period.intersects(cp._period); }
    /** @return true if the time_period contains a ptime */
    bool contains(boost::posix_time::ptime const &t) const { return _period.contains(t); }
    /** @brief the capacity */
    long _capa;
    /** @brief the time_period */
    time_period _period;
    /** @brief equality operator */
    bool operator==(capa_period const &cp) const {return _capa==cp._capa && _period == cp._period;}
};

/**
 * @typedef LTimePeriod
 * @brief list of period
 * @addtogroup period_type_def
 */
typedef std::list<time_period> LTimePeriod;
/**
 * @typedef LCapaPeriod
 * @brief list of period with capacity
 * @addtogroup period_type_def
 */
typedef std::list<capa_period> LCapaPeriod;

/**
 * @brief OStream operator for capa_period
 * @param os the ostream
 * @param cp the capa_period
 */
std::ostream &operator<<(std::ostream &os, capa_period const &cp);


///////////
// Union //
///////////
/**
 * @brief Compute the union of two lists of Periods
 * @tparam PeriodT the type of periods
 * @param periods1 first list
 * @param periods2 second list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the lists must be sorted and contain disjoint periods
 * @return the union
 */
template<typename PeriodT>
std::list<PeriodT> get_union(std::list<PeriodT> const &periods1, std::list<PeriodT> const &periods2, bool merge_adjacent=true);
/**
 * @brief Compute the union of a period and a list of periods
 * @tparam PeriodT the type of periods
 * @param period the period
 * @param periods the list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the union
 */
template<typename PeriodT>
std::list<PeriodT> get_union(PeriodT const &period, std::list<PeriodT> const& periods, bool merge_adjacent=true)
{
    return get_union(std::list<PeriodT>(1,period), periods, merge_adjacent);
}

/**
 * @brief Compute the union of a list of periods and a period
 * @tparam PeriodT the type of periods
 * @param period the period
 * @param periods the list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the union
 */
template<typename PeriodT>
std::list<PeriodT> get_union(std::list<PeriodT> const &periods, PeriodT const &period, bool merge_adjacent=true)
{
    return get_union(periods, std::list<PeriodT>(1,period), merge_adjacent);
}
/**
 * @brief Compute the union of two periods
 * @tparam PeriodT the type of periods
 * @param period1 the first period
 * @param period2 the second period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the union
 */
template<typename PeriodT>
std::list<PeriodT> get_union(PeriodT const &period1, PeriodT const& period2, bool merge_adjacent=true)
{
    return get_union(std::list<PeriodT>(1,period1), std::list<PeriodT>(1, period2), merge_adjacent);
}

////////////////
// Difference //
////////////////
/**
 * @brief Compute the difference of two lists of Periods
 * @tparam PeriodT the type of periods
 * @param periods1 first list
 * @param periods2 second list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the lists must be sorted and contain disjoint periods
 * @return the difference
 */
template<typename PeriodT>
std::list<PeriodT> get_diff(std::list<PeriodT> const &periods1, std::list<PeriodT> const &periods2, bool merge_adjacent=true);
/**
 * @brief Compute the difference of a period and a list of periods
 * @tparam PeriodT the type of periods
 * @param period the period
 * @param periods the list
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the difference
 */
template<typename PeriodT>
std::list<PeriodT> get_diff(PeriodT const &period, std::list<PeriodT> const& periods, bool merge_adjacent=true)
{
    return get_diff(std::list<PeriodT>(1,period), periods, merge_adjacent);
}
/**
 * @brief Compute the difference of a list of periods and a period
 * @tparam PeriodT the type of periods
 * @param periods the list
 * @param period the period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the difference
 */
template<typename PeriodT>
std::list<PeriodT> get_diff(std::list<PeriodT> const &periods, PeriodT const &period, bool merge_adjacent=true)
{
    return get_diff(periods, std::list<PeriodT>(1,period), merge_adjacent);
}
/**
 * @brief Compute the difference of two periods
 * @tparam PeriodT the type of periods
 * @param period1 the first period
 * @param period2 the second period
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the list must be sorted and contain disjoint periods
 * @return the difference
 */
template<typename PeriodT>
std::list<PeriodT> get_diff(PeriodT const &period1, PeriodT const& period2, bool merge_adjacent=true)
{
    return get_diff(std::list<PeriodT>(1,period1), std::list<PeriodT>(1, period2), merge_adjacent);
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
 * @brief Compute the intersection of two periods
 * @tparam PeriodT the type of first periods
 * @tparam PeriodU the type of second periods
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

//Implementation
#include <utils/period/period_utils.tpp>

