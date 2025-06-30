#include <nael_utils/period/period_utils.hh>

#include <nael_utils/safe_comp/safe_comp.hh>
#include <algorithm>

namespace bpt = boost::posix_time;

/**
 * @brief check that two time_period can be merged
 * @param tp1 the first period
 * @param tp2 the second period
 * @return true if p1 and p2 are adjacent with some tolerance
 */
bool can_merge(time_period const &tp1, time_period const &tp2)
{
    return is_same(tp1.end(), tp2.begin()) || is_same(tp2.end(), tp1.begin());
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

//Compute a relative duration
boost::posix_time::time_duration compute_relative_duration(boost::posix_time::time_duration duration, float ratio)
{
    return bpt::milliseconds(
        long(
            std::round(
                double(duration.total_milliseconds())*ratio
            )
        )
    );
}

//Compute an absolute duration
boost::posix_time::time_duration compute_theoretical_duration(boost::posix_time::time_duration duration, float ratio)
{
    return bpt::milliseconds(
        long(
            std::round(
                double(duration.total_milliseconds())/ratio
            )
        )
    );
}

//Compute the relative duration of a list of periods
bpt::time_duration get_relative_duration(LRatioPeriod const &periods)
{
    return std::accumulate(
            periods.begin(), periods.end(), bpt::time_duration(0,0,0),
            [](bpt::time_duration duration, ratio_period const& period)
            {
                return duration + period.get_relative_duration();
            } );
}

// Checks that two ptime are the same (with a tolerance)
bool is_same(bpt::ptime const &lhs, bpt::ptime const &rhs, bpt::time_duration const &tol)
{
    if(lhs.date() != rhs.date())
    {
        return false;
    }
    bpt::time_duration diff = (lhs > rhs)
        ? lhs - rhs
        : rhs - lhs;

    return diff <= tol;
}
