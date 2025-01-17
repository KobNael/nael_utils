#include <nael_utils/period/period_utils.hh>

/**
 * @brief check that two time_period can be merged
 * @param tp1 the first period
 * @param tp2 the second period
 * @return true if p1 and p2 are adjacent
 */
bool can_merge(time_period const&tp1, time_period const &tp2)
{
    return tp1.end() == tp2.begin() || tp2.end() == tp1.begin();
}

/**
 * @brief check that two capa_period can be merged
 * @param cp1 the first period
 * @param cp2 the second period
 * @return true if p1 and p2 have the same capa and adjacent periods
 */
bool can_merge(capa_period const&cp1, capa_period const &cp2)
{
    return cp1._capa == cp2._capa && can_merge(cp1._period, cp2._period);
}

/**
 * @brief merge two time periods
 * @param tp1 the first period
 * @param tp2 the second period
 * @pre the periods can be merged, ie. are adjacent
 * @return the result of the merge
 */
time_period merge(time_period const&tp1, time_period const &tp2)
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
capa_period merge(capa_period const&cp1, capa_period const &cp2)
{
    assert(can_merge(cp1, cp2));
    return capa_period(cp1._capa, cp1._period.span( cp2._period ));
}
