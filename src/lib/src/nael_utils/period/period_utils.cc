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
    return tp1.span(tp2);
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
    return capa_period(cp1._capa, cp1._period.span(cp2._period));
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
    return ratio_period(rp1._ratio, rp1._period.span(rp2._period));
}

// Collect the capa_period with enough capacity and return them as time_period
LTimePeriod get_eligible_periods(LCapaPeriod const &periods, int64_t min)
{
    LTimePeriod result;
    std::ranges::for_each(
        periods,
        [min, &result](capa_period const &cp)
        {
            if (cp._capa >= min)
            {
                result.push_back(cp._period);
            }
        });
    merge_adjacent_periods(result);
    return result;
}

// Convert a list of time_period into a list of capa_period with a given capacity
LCapaPeriod convert(LTimePeriod const &periods, int64_t capa)
{
    LCapaPeriod result;
    std::ranges::transform(
        periods,
        std::back_inserter(result),
            [capa](auto const& period) { return capa_period(capa, period); });
    return result;
}

// Compute a relative duration
boost::posix_time::time_duration compute_relative_duration(boost::posix_time::time_duration duration, float ratio)
{
    return bpt::milliseconds(
        int64_t(
            std::round(
                double(duration.total_milliseconds()) * ratio)));
}

// Compute an absolute duration
boost::posix_time::time_duration compute_theoretical_duration(boost::posix_time::time_duration duration, float ratio)
{
    return bpt::milliseconds(
        int64_t(
            std::round(
                double(duration.total_milliseconds()) / ratio)));
}

// Reduce a list of periods from the left by a given duration taking into account the ratio of each period
LRatioPeriod reduce_left(LRatioPeriod const &periods, boost::posix_time::time_duration const &duration, std::vector<bool> const &can_be_partially_reduced)
{
    LRatioPeriod result;
    if(periods.empty())
    {
        return result;
    }
    // iterate over the periods and search for the cut point
    auto it = periods.begin();
    size_t i(0);
    boost::posix_time::time_duration remaining_duration = duration;
    while(it != periods.end() && remaining_duration > bpt::time_duration(0,0,0))
    {
        bool period_can_be_partially_reduced = can_be_partially_reduced.size() <= i || can_be_partially_reduced.at(i);
        ++i;
        // not enough time or partial reduction not allowed, go on
        if(auto rel_dur = it->get_relative_duration(); rel_dur <= remaining_duration || !period_can_be_partially_reduced)
        {
            remaining_duration -= rel_dur;
            ++it;
            continue;
        }
        // cut in this period and insert it
        bpt::time_duration absolute_reduction = compute_theoretical_duration(remaining_duration, it->_ratio);
        result.emplace_back(
            it->_ratio,
            it->begin() + absolute_reduction,
            it->end());
        ++it;
        break;
    }
    // copy the remaining periods
    result.insert(result.end(), it, periods.end());
    return result;
}

// Reduce a list of periods from the right by a given duration taking into account the ratio of each period
LRatioPeriod reduce_right(LRatioPeriod const &periods, boost::posix_time::time_duration const &duration, std::vector<bool> const &can_be_partially_reduced)
{
    LRatioPeriod result;
    if(periods.empty())
    {
        return result;
    }
    // iterate over the periods and search for the cut point
    auto it = periods.rbegin();
    size_t i(periods.size() - 1);
    boost::posix_time::time_duration remaining_duration = duration;
    while(it != periods.rend() && remaining_duration > bpt::time_duration(0,0,0))
    {
        bool period_can_be_partially_reduced = can_be_partially_reduced.size() <= i || can_be_partially_reduced.at(i);
        --i;
        // not enough time or partial reduction not allowed, go on
        if(auto rel_dur = it->get_relative_duration(); rel_dur <= remaining_duration || !period_can_be_partially_reduced)
        {
            remaining_duration -= rel_dur;
            ++it;
            continue;
        }
        // cut in this period and insert it
        bpt::time_duration absolute_reduction = compute_theoretical_duration(remaining_duration, it->_ratio);
        result.emplace_back(
            it->_ratio,
            it->begin(),
            it->end() - absolute_reduction);
        ++it;
        break;
    }
    // copy the remaining periods
    result.insert(result.end(), it, periods.rend());
    std::ranges::reverse(result);
    return result;
}

// Compute the relative duration of a list of periods
bpt::time_duration get_relative_duration(LRatioPeriod const &periods)
{
    return std::accumulate(
        periods.begin(), periods.end(), bpt::time_duration(0, 0, 0),
        [](bpt::time_duration duration, ratio_period const &period)
        {
            return duration + period.get_relative_duration();
        });
}

// Compute the earliest end date such that the relative duration of the periods between the start date and this end date is equal to a given duration
boost::posix_time::ptime get_earliest_end_date_from_duration(LRatioPeriod const &periods, boost::posix_time::time_duration const &duration)
{
    // check consistency
    if(periods.empty())
    {
        return bpt::not_a_date_time;
    }

    // no duration
    if(duration <= bpt::seconds(0))
    {
        return periods.front().begin();
    }

    // iterate over the periods and search for the cut point
    bpt::time_duration remaining_duration = duration;
    for(auto const &period : periods)
    {
        // skip invalid ratio
        if(safecomp::le(period._ratio, .0f))
        {
            continue;
        }
        // not enough time, reduce the target and go on
        if(auto rel_dur = period.get_relative_duration(); rel_dur < remaining_duration)
        {
            remaining_duration -= rel_dur;
            continue;
        }
        // cut in this period and return the end date
        bpt::time_duration absolute_missing_duration = compute_theoretical_duration(remaining_duration, period._ratio);
        return period.begin() + absolute_missing_duration;
    }
    return bpt::not_a_date_time;
}

// Compute the earliest end date such that the relative duration (by multiplying the duration of each period by its capacity) of the periods between the start date and this end date is equal to a given duration
boost::posix_time::ptime get_earliest_end_date_from_duration(LCapaPeriod const &periods, boost::posix_time::time_duration const &duration)
{
    // check consistency
    if(periods.empty())
    {
        return bpt::not_a_date_time;
    }

    // no duration
    if(duration <= bpt::seconds(0))
    {
        return periods.front().begin();
    }

    // iterate over the periods and search for the cut point
    bpt::time_duration remaining_duration = duration;
    for(auto const &period : periods)
    {
        // skip invalid capacity
        if(period._capa <= 0)
        {
            continue;
        }
        // not enough time, go on
        if(auto rel_dur = period.length()*period._capa; rel_dur < remaining_duration)
        {
            remaining_duration -= rel_dur;
            continue;
        }
        // cut in this period and return the end date
        bpt::time_duration absolute_missing_duration = remaining_duration / period._capa;
        return period.begin() + absolute_missing_duration;
    }
    return bpt::not_a_date_time;
}

// Create an artificial empty period
time_period make_empty_period(boost::posix_time::ptime ptime)
{
    return time_period(ptime, ptime + bpt::milliseconds(10));
}

// Checks that two ptime are the same (with a tolerance)
bool is_same(bpt::ptime const &lhs, bpt::ptime const &rhs, bpt::time_duration const &tol)
{
    bpt::time_duration diff = (lhs > rhs)
                                  ? lhs - rhs
                                  : rhs - lhs;
    return diff <= tol;
}

// Checks that two durations are the same (with a tolerance)
bool is_same(boost::posix_time::time_duration const &lhs, boost::posix_time::time_duration const &rhs, boost::posix_time::time_duration const &tol)
{
    boost::posix_time::time_duration diff = (lhs > rhs) ? lhs - rhs : rhs - lhs;
    return diff <= tol;
}

// Checks that two periods are the same (with a tolerance)
bool is_same(boost::posix_time::time_period const &lhs, boost::posix_time::time_period const &rhs, boost::posix_time::time_duration const &tol)
{
    return is_same(lhs.begin(), rhs.begin(), tol) && is_same(lhs.end(), rhs.end(), tol);
}
