/**
 * @file period_generation.cc
 */

#include <list>
#include <nael_utils/period/period_generation.hh>

namespace
{
    ///////////////////
    // Split horizon //
    ///////////////////
    /**
     * @brief Generate the time period defined by a couple start/end on an horizon
     * @tparam PeriodT the type of period to generate
     * @tparam FunT a functor able to generate a PeriodT given it start/end time
     * @param start_horizon starting timestamp
     * @param end_horizon ending timestamp
     * @param start_period the starting time of the period
     * @param end_period the ending time of the period
     * @param fun the functor
     * @pre end_horizon > start_horizon
     * @pre end_horizon and start_horizon are valid date times
     * @pre 00:00:00 <= start_period <= 23:59:59
     * @pre 00:00:00 <= end_period <= 23:59:59
     */
    template <class PeriodT, class FunT>
    std::list<PeriodT> generate_periods(bpt::ptime const &start_horizon, bpt::ptime const &end_horizon, bpt::time_duration const &start_period, bpt::time_duration const &end_period, FunT fun)
    {
        // Pre conditions
        assert(!start_horizon.is_not_a_date_time() && !end_horizon.is_not_a_date_time());
        assert(start_horizon < end_horizon);
        assert(bpt::seconds(0) <= start_period && start_period <= bpt::time_duration(23, 59, 59));
        assert(bpt::seconds(0) <= end_period && end_period <= bpt::time_duration(23, 59, 59));

        // Init the result
        std::list<PeriodT> result;
        // Generate the first period according to the time
        bg::date first_day(start_horizon.date());
        PeriodT period =
            start_period <= end_period
                ? fun(bpt::ptime(first_day, start_period), bpt::ptime(first_day, end_period))
                : fun(bpt::ptime(first_day - bg::days(1), start_period), bpt::ptime(first_day, end_period));

        // Push has many periods as necessary
        do
        {
            result.push_back(period);
            period.shift(bpt::hours(24));
        } while (period.begin() <= end_horizon);
        // Return the result, limited to the horizon
        return get_inter(result, time_period(start_horizon, end_horizon));
    }

    template<typename DateIteratorT>
    LTimePeriod
    generate_periods(bpt::ptime const &start_horizon, bpt::ptime const &end_horizon, bg::date const &start_date, DateIteratorT it)
    {
        LTimePeriod result;
        bg::date cur_date = start_date;
        do
        {
            ++it;
            result.push_back(
                time_period(
                    bpt::ptime(cur_date, bpt::hours(0)), bpt::ptime(*it - bg::days(1), bpt::time_duration(23, 59, 59))));
            cur_date = *it;
        } while (cur_date <= end_horizon.date());
        return get_inter(result, time_period(start_horizon, end_horizon));
    }
} // anonymous namespace

// Generate the time period defined by a couple start/end on an horizon
LTimePeriod
    generate_periods(bpt::ptime const &start_horizon, bpt::ptime const &end_horizon, TemporalMesh const &temporal_mesh)
{
    switch (temporal_mesh)
    {
    case TemporalMesh::HORIZON:
        return LTimePeriod(1, time_period(start_horizon, end_horizon));
    case TemporalMesh::DAY:
    {
        // get the first day of the week
        auto const &start{start_horizon.date()};
        return generate_periods<bg::day_iterator>(start_horizon, end_horizon, start, bg::day_iterator(start, 1));
    }
    case TemporalMesh::WEEK:
    {
        // get the first day of the week
        auto generator{bg::first_day_of_the_week_before(bg::Monday)};
        auto const &start{generator.get_date(start_horizon.date())};
        return generate_periods<bg::day_iterator>(start_horizon, end_horizon, start, bg::day_iterator(start, 7));
    }
    case TemporalMesh::MONTH:
    {
        // get the first day of the month
        auto const &start{bg::date(start_horizon.date().year(), start_horizon.date().month(), 1)};
        return generate_periods<bg::month_iterator>(start_horizon, end_horizon, start, bg::month_iterator(start));
    }
    }
    return LTimePeriod();
}

// Generate the time period defined by a couple start/end on an horizon
LTimePeriod generate_periods(bpt::ptime const &start_horizon, bpt::ptime const &end_horizon, bpt::time_duration const &start_period, bpt::time_duration const &end_period)
{
    return generate_periods<time_period>(
        start_horizon, end_horizon, start_period, end_period,
        [](boost::posix_time::ptime const &start, boost::posix_time::ptime const &end)
        {
            return time_period(start, end);
        });
}

// Generate the capacitated time period defined by a couple start/end on an horizon
LCapaPeriod generate_periods(bpt::ptime const &start_horizon, bpt::ptime const &end_horizon, bpt::time_duration const &start_period, bpt::time_duration const &end_period, long capa)
{
    return generate_periods<capa_period>(
        start_horizon, end_horizon, start_period, end_period,
        [capa](boost::posix_time::ptime const &start, boost::posix_time::ptime const &end)
        {
            return capa_period(capa, start, end);
        });
}

