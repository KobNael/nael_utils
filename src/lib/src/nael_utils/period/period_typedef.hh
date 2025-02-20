#pragma once

#include <list>
#include <boost/date_time/posix_time/posix_time.hpp>

/**
 * @file period_utils.hh
 * @brief Toolbox for manipulation of time period.   
 * Two types of time period are proposed, along with shortcuts for list of periods:
 * - #time_period : shortcut for boost::posix_time::time_period
 * - capa_period : #time_period with capacity (long long)
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
using time_period = boost::posix_time::time_period;

/**
 * @struct capa_period
 * @brief Represents a capacity on a time_period
 * @addtogroup period_type_def
 */
struct capa_period
{
    public:
        /**
         * @brief constructor
         * @param capa the capacity
         * @param period the time_period
         */
        capa_period(long long capa, time_period const &period)
            : _capa(capa)
            , _period(period)
        {}
        /**
         * @brief constructor
         * @param capa the capacity
         * @param start the starting date time
         * @param end the ending date time
         */
        capa_period(long long capa, boost::posix_time::ptime const &start, boost::posix_time::ptime const &end)
            : _capa(capa)
            , _period(time_period(start, end))
        {}
        /** @return the begin of the period */
        boost::posix_time::ptime begin() const { return _period.begin(); }
        /** @return the end of the period */
        boost::posix_time::ptime end() const { return _period.end(); }
        /** @return the period duration */
        boost::posix_time::time_duration length() const { return _period.length(); }
        /** @brief Add duration to both begin and end. */
        void shift(boost::posix_time::time_duration const &d) { return _period.shift(d); }
        /** @return true if the time_period intersects another time_period */
        bool intersect(capa_period const &cp) const { return _period.intersects(cp._period); }
        /** @return true if the time_period contains a ptime */
        bool contains(boost::posix_time::ptime const &t) const { return _period.contains(t); }
        /** @brief the capacity */
        long long _capa;
        /** @brief the time_period */
        time_period _period;
        /** @brief equality operator */
        bool operator==(capa_period const &cp) const = default;
    private:
        /**
         * @brief OStream operator for capa_period
         * @param os the ostream
         * @param cp the capa_period
         */
        friend std::ostream &operator<<(std::ostream &os, capa_period const &cp)
        {
            return os << "(" << cp._period << "/" << cp._capa << ")";
        }
};

/**
 * @typedef LTimePeriod
 * @brief list of period
 * @addtogroup period_type_def
 */
using LTimePeriod = std::list<time_period>;
/**
 * @typedef LCapaPeriod
 * @brief list of period with capacity
 * @addtogroup period_type_def
 */
using LCapaPeriod = std::list<capa_period>;
