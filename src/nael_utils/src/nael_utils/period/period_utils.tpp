/** @file */
#include <optional>
#include <nael_utils/period/period_functor.hh>

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

/**
 * @brief Browse a list of periods and merge the adjacent one
 * @tparam PeriodT the type of periods
 * @param[out] periods the list of periods
 * @pre the lists must be sorted and contain disjoint periods
 */
template<typename PeriodT>
void merge_adjacent_periods(std::list<PeriodT> &periods)
{
    auto it = periods.begin();
    //for each pair of period
    while(std::next(it) != periods.end())
    {
        //If we can merge it
        if(can_merge(*std::next(it), *it))
        {
            //do it
            *it = merge(*it, *std::next(it));
            //and remove the next one
            periods.erase(std::next(it));
        }
        //otherwise advance
        else
        {
            ++it;
        }
    }
}

#ifndef NDEBUG
/**
 * @brief Check that a list of periods is valid
 * @tparam PeriodT the type of periods
 * @param periods the list of periods
 * @throw if the list is sorted or contains overlapping periods
 */
template<typename PeriodT>
void check_vector(std::list<PeriodT> const&periods)
{
    auto it = periods.begin();
    //for each pair of period
    while(std::next(it) != periods.end())
    {
        if(it->end() > std::next(it)->begin())
        {
            throw std::logic_error("Invalid list of periods");
        }
        ++it;
    }
}
#endif

/**
 * @brief Insert a period in a list if needed
 * @tparam PeriodT the type of period
 * @param p the period
 * @param[out] list the list
 */
template<typename PeriodT>
void opt_insert(std::optional<PeriodT> p, std::list<PeriodT> &list)
{
    if(p)
    {
        list.push_back(*p);
    }
}
/**
 * @brief Browse two lists and apply a functor on each intersection
 * @tparam PeriodT the type of periods
 * @tparam PeriodU the type of periods
 * @tparam FuncT the type of functor
 * @param periods1 the first list of periods
 * @param periods2 the second list of periods
 * @param merge_adjacent should we merge the adjacent periods
 * @pre the lists must be sorted and contain disjoint periods
 * @return the result of the merge
 */
template<typename PeriodT, typename PeriodU, typename FuncT>
std::list<PeriodT> merge(std::list<PeriodT> const&periods1, std::list<PeriodU> const& periods2, bool merge_adjacent)
{
    namespace bpt = boost::posix_time;
    std::list<PeriodT> res;
#ifndef NDEBUG
    check_vector(periods1);
    check_vector(periods2);
#endif
    //Check vectors
    //Create functor
    FuncT func;
    (void) func;
    //Browse lists
    auto it1 = periods1.begin();
    auto it2 = periods2.begin();
    bpt::ptime curtime = (it1 != periods1.end() && it2 != periods2.end())
        ? std::min(it1->begin(), it2->begin())
        : bpt::not_a_date_time;
    while(it1 != periods1.end() && it2 != periods2.end())
    {
        //if periods does not intersect
        if(it1->end() <= it2->begin() || it2->end() <= it1->begin())
        {
            //take the earliest one, and apply the functor on it
            if(it1->end() <= it2->begin())
            {
                curtime = std::max(curtime, it1->begin());
                opt_insert<PeriodT>(func(*it1, (std::optional<PeriodU>){}, time_period(curtime, it1->end())), res);
                curtime = it1->end();
                ++it1;
            }
            else
            {
                curtime = std::max(curtime, it2->begin());
                opt_insert<PeriodT>(func((std::optional<PeriodT>){}, *it2, time_period(curtime, it2->end())), res);
                curtime = it2->end();
                ++it2;
            }
        }
        else
        {
            //Get the intersection, taking curtime into account
            bpt::ptime begin = std::max(curtime, std::max( it1->begin(), it2->begin() ) )
                    , end = std::min( it1->end(), it2->end() );
            //first part if any
            if(curtime < begin)
            {
                if(it1->begin() < begin)
                {
                    opt_insert<PeriodT>(func(*it1, (std::optional<PeriodU>){}, time_period(curtime, begin)), res);
                }
                if(it2->begin() < begin)
                {
                    opt_insert<PeriodT>(func((std::optional<PeriodT>){}, *it2, time_period(curtime, begin)), res);
                }
                curtime = begin;
            }
            //intersection
            opt_insert<PeriodT>(func(*it1, *it2, time_period(begin, end)), res);
            //update curtime
            curtime = end;
            //increase iterator accordingly
            if(curtime >= it1->end())
            {
                ++it1;
            }
            if(curtime >= it2->end())
            {
                ++it2;
            }

        }
    }
    //Handle the remaining
    curtime = (curtime.is_not_a_date_time())
        ? bpt::min_date_time
        : curtime;
    while(it1 != periods1.end())
    {
        opt_insert<PeriodT>(func(*it1, (std::optional<PeriodU>){}, time_period(std::max(curtime, it1->begin()), it1->end())), res);
        curtime = it1->end();
        ++it1;
    }
    while(it2 != periods2.end())
    {
        opt_insert<PeriodT>(func((std::optional<PeriodT>){}, *it2, time_period(std::max(curtime, it2->begin()), it2->end())), res);
        curtime = it2->end();
        ++it2;
    }

    //If we must merge the adjacent periods
    if(merge_adjacent)
    {
        merge_adjacent_periods(res);
    }
    return res;
}

/* Compute the union of two lists of Periods */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_union(std::list<PeriodT> const&periods1, std::list<PeriodU> const& periods2, bool merge_adjacent)
{
    return merge<PeriodT, PeriodU, details::MakeUnion>(periods1, periods2, merge_adjacent);
}

/* Compute the difference of two lists of Periods */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_diff(std::list<PeriodT> const&periods1, std::list<PeriodU> const& periods2, bool merge_adjacent)
{
    return merge<PeriodT, PeriodU, details::MakeDiff>(periods1, periods2, merge_adjacent);
}


/* Compute the intersection of two lists of Periods */
template<typename PeriodT, typename PeriodU>
std::list<PeriodT> get_inter(std::list<PeriodT> const&periods1, std::list<PeriodU> const& periods2, bool merge_adjacent)
{
    return merge<PeriodT, PeriodU, details::MakeInter>(periods1, periods2, merge_adjacent);
}

