#pragma once

#include <list>
#include <boost/date_time/posix_time/posix_time.hpp>

///////////
// Types //
///////////
/** @brief Alias for time period */
typedef boost::posix_time::time_period time_period;

/** @struct CapaPeriod */
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
 * @brief OStream operator for capa_period
 * @param os the ostream
 * @param p the capa_period
 */
std::ostream &operator<<(std::ostream &os, capa_period const &cp);

/** @brief list of period */
typedef std::list<time_period> LTimePeriod;
/** @brief list of period with capacity */
typedef std::list<capa_period> LCapaPeriod;


///////////
// Merge //
///////////
/**
 * @brief check that two time_period can be merged
 * @return true if p1 and p2 are adjacent
 */
bool can_merge(time_period const&tp1, time_period const &tp2);
/**
 * @brief check that two capa_period can be merged
 * @return true if p1 and p2 have the same capa and adjacent periods
 */
bool can_merge(capa_period const&cp1, capa_period const &cp2);
/**
 * @return the result of the merge of two time_period
 * @pre the periods can be merged
 */
time_period merge(time_period const&tp1, time_period const &tp2);
/**
 * @return the result of the merge of two capa_period
 * @pre the periods can be merged
 */
capa_period merge(capa_period const&cp1, capa_period const &cp2);

///////////
// Union //
///////////
/**
 * @brief compute the union of two lists of Periods
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
 * @brief compute the union of a period and a list of periods
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
 * @brief compute the union of a list of periods and a period
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
 * @brief compute the union of two periods
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
 * @brief compute the difference of two lists of Periods
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
 * @brief compute the difference of a period and a list of periods
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
 * @brief compute the difference of a list of periods and a period
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
 * @brief compute the difference of two periods
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
 * @brief compute the intersection of two lists of Periods
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
 * @brief compute the intersection of a period and a list of periods
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
 * @brief compute the intersection of a list of periods and a period
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
 * @brief compute the intersection of two periods
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
