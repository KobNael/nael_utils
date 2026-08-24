/**
 * @file period_generation.cc
 */

#include <list>
#include <nael_utils/period/period_generation.hh>

namespace bg = boost::gregorian;

namespace
{

    template<typename DateIteratorT>
    LTimePeriod
    generate_periods(boost::posix_time::ptime const &start_horizon, boost::posix_time::ptime const &end_horizon, boost::gregorian::date const &start_date, boost::posix_time::time_duration const &offset, DateIteratorT it)
    {
        LTimePeriod result;
        boost::gregorian::date cur_date = start_date;
        do
        {
            ++it;
            // create the period
            time_period period(boost::posix_time::ptime(cur_date, boost::posix_time::hours(0)), boost::posix_time::ptime(*it, boost::posix_time::hours(0)));
            // handle the offset
            period.shift(offset);
            // store it
            result.push_back(period);
            // go on
            cur_date = *it;
        } while (cur_date <= end_horizon.date());
        return get_inter(result, time_period(start_horizon, end_horizon), false);
    }
} // anonymous namespace

// Generate the time period defined by a couple start/end on an horizon
LTimePeriod
    generate_periods(boost::posix_time::ptime const &start_horizon, boost::posix_time::ptime const &end_horizon, TemporalMesh const &temporal_mesh, boost::posix_time::time_duration const &offset)
{
    // Pre conditions
    assert(!start_horizon.is_not_a_date_time() && !end_horizon.is_not_a_date_time());
    assert(start_horizon < end_horizon);
    assert(boost::posix_time::hours(-24) < offset && offset < boost::posix_time::hours(24));
    // Generation
    switch (temporal_mesh)
    {
    case TemporalMesh::NONE:
        return LTimePeriod();
    case TemporalMesh::HORIZON:
        return LTimePeriod(1, time_period(start_horizon, end_horizon));
    case TemporalMesh::DAY:
    {
        // get the first day of the week
        auto const &start{start_horizon.date()};
        return generate_periods<boost::gregorian::day_iterator>(start_horizon, end_horizon, start, offset, boost::gregorian::day_iterator(start, 1));
    }
    case TemporalMesh::WEEK:
    {
        // get the first day of the week
        auto generator{boost::gregorian::first_day_of_the_week_before(boost::gregorian::Monday)};
        auto const &start{generator.get_date(start_horizon.date())};
        return generate_periods<boost::gregorian::day_iterator>(start_horizon, end_horizon, start, offset, boost::gregorian::day_iterator(start, 7));
    }
    case TemporalMesh::MONTH:
    {
        // get the first day of the month
        auto const &start{boost::gregorian::date(start_horizon.date().year(), start_horizon.date().month(), 1)};
        return generate_periods<boost::gregorian::month_iterator>(start_horizon, end_horizon, start, offset, boost::gregorian::month_iterator(start));
    }
    }
    return LTimePeriod();
}
