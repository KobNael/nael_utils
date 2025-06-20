#include <nael_utils/period/period_utils.hh>

#include <nael_utils/safe_comp/safe_comp.hh>
#include <algorithm>

namespace bpt = boost::posix_time;

/**
 * @brief check that two time_period can be merged
 * @param tp1 the first period
 * @param tp2 the second period
 * @return true if p1 and p2 are adjacent
 */
bool can_merge(time_period const &tp1, time_period const &tp2)
{
    return tp1.end() == tp2.begin() || tp2.end() == tp1.begin();
}

/**
 * @brief check that two capa_period can be merged
 * @param cp1 the first period
 * @param cp2 the second period
 * @return true if p1 and p2 have the same capa and adjacent periods
 */
bool can_merge(capa_period const &cp1, capa_period const &cp2)
{
    return cp1._capa == cp2._capa && can_merge(cp1._period, cp2._period);
}

/**
 * @brief check that two ratio_period can be merged
 * @param rp1 the first period
 * @param rp2 the second period
 * @return true if p1 and p2 have the same ratio and adjacent periods
 */
bool can_merge(ratio_period const &rp1, ratio_period const &rp2)
{
    return safecomp::eq<float>(rp1._ratio, rp2._ratio) && can_merge(rp1._period, rp2._period);
}

/**
 * @brief merge two time periods
 * @param tp1 the first period
 * @param tp2 the second period
 * @pre the periods can be merged, ie. are adjacent
 * @return the result of the merge
 */
time_period merge(time_period const &tp1, time_period const &tp2)
{
    assert(can_merge(tp1, tp2));
    return tp1.span( tp2 );
}

/**
 * @brief merge two capa_period
 * @param cp1 the first period
 * @param cp2 the second period
 * @pre the periods can be merged, ie. are adjacent and have the same capa
 * @return the result of the merge
 */
capa_period merge(capa_period const &cp1, capa_period const &cp2)
{
    assert(can_merge(cp1, cp2));
    return capa_period(cp1._capa, cp1._period.span( cp2._period ));
}

/**
 * @brief merge two ratio_period
 * @param rp1 the first period
 * @param rp2 the second period
 * @pre the periods can be merged, ie. are adjacent and have the same ratio
 * @return the result of the merge
 */
ratio_period merge(ratio_period const &rp1, ratio_period const &rp2)
{
    assert(can_merge(rp1, rp2));
    return ratio_period(rp1._ratio, rp1._period.span( rp2._period ));
}

// Collect the capa_period with enough capacity and return them as time_period
LTimePeriod get_eligible_periods(LCapaPeriod const &periods, unsigned min)
{
    LTimePeriod result;
    std::for_each(
        periods.cbegin(), periods.cend(),
        [min, &result](capa_period const &cp)
        {
            if(cp._capa >= min)
            {
                result.push_back(cp._period);
            }
        });
    return result;
}

//Compute the relative duration of a list of periods
boost::posix_time::time_duration get_relative_duration(LRatioPeriod const &periods)
{
    boost::posix_time::time_duration relative_duration(0,0,0);
    //Get the total (ponderated) duration in milli sec
    double total_millisec =
        std::accumulate(
            periods.begin(), periods.end(), double(0),
            [](double total, ratio_period const& period)
            {
                return total + double(period.length().total_milliseconds())*period._ratio;
            } );
    //Convert into a duration
    return bpt::milliseconds( long(std::floor(total_millisec)) );
}
