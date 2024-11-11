/**
 * @file period_functor.hh
 */
#include <optional>

namespace details
{
/**
 * @struct MakeUnion
 * @brief Functor making the union of two (capa) periods
 */
struct MakeUnion
{
    /**
     * @brief Compute the union between two optional periods on a given period
     * @param p1 the first period
     * @param p2 the second period
     * @param period the time_period
     */
    time_period operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
    {
        (void)p1;
        (void)p2;
        assert(p1 || p2);
        //if p1 or p2, return the period
        return period;
    }
    /**
     * @brief Compute the union between two optional capa_periods on a given period
     * @param p1 the first capa_period
     * @param p2 the second capa_period
     * @param period the time_period
     */
    std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
    {
        //compute the capacity
        long capa = ((p1)?p1->_capa:0)+((p2)?p2->_capa:0);
        if(0 != capa)
        {
            return capa_period(capa, period);
        }
        return {};
    }
};

/**
 * @struct MakeDiff
 * @brief Functor making the difference of two (capa) periods
 */
struct MakeDiff
{
    /**
     * @brief Compute the difference between two optional periods on a given period
     * @param p1 the first period
     * @param p2 the second period
     * @param period the time_period
     */
    std::optional<time_period> operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
    {
        //if !p1 or both periods return nothing
        if(!p1 || p2)
        {
            return {};
        }
        //if only p1 return the full period
        else
        {
            return period;
        }
    }
    /**
     * @brief Compute the difference between two optional capa_periods on a given period
     * @param p1 the first capa_period
     * @param p2 the second capa_period
     * @param period the time_period
     */
    std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
    {
        //compute the remaining capacity
        long capa = ((p1)?p1->_capa:0)-((p2)?p2->_capa:0);
        if(0 != capa)
        {
            return capa_period(capa, period);
        }
        return {};
    }
};

/**
 * @struct MakeInter
 * @brief Functor making the intersection of two (capa) periods
 */
struct MakeInter
{
    /**
     * @brief Compute the intersection between two optional periods on a given period
     * @param p1 the first period
     * @param p2 the second period
     * @param period the time_period
     */
    std::optional<time_period>
    operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
    {
        //if only p1 or p2, return nothing
        if(!p1 || !p2)
        {
            return {};
        }
        //if both, check that tey are the same and return
        else
        {
            return period;
        }
    }
    /**
     * @brief Compute the intersection between two optional capa_periods on a given period
     * @param p1 the first capa_period
     * @param p2 the second capa_period
     * @param period the time_period
     */
    std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
    {
        //if only p1 or p2, return nothing
        if(!p1 || !p2)
        {
            return {};
        }
        else
        {
            //return the period with the min capa
            long capa( std::min((*p1)._capa, (*p2)._capa) );
            if(0 != capa)
            {
                return capa_period(capa, period);
            }
            return {};
        }
    }
    /**
     * @brief Compute the intersection between two optional periods on a given period
     * @param p1 the first period
     * @param p2 the second period
     * @param period the time_period
     */
    std::optional<time_period>
    operator()(std::optional<time_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
    {
        //if only p1 or p2, return nothing
        if(!p1 || !p2)
        {
            return {};
        }
        //if both, check that tey are the same and return
        else
        {
            return period;
        }
    }
    /**
     * @brief Compute the intersection between two optional capa_periods on a given period
     * @param p1 the first capa_period
     * @param p2 the second capa_period
     * @param period the time_period
     */
    std::optional<capa_period> operator()(std::optional<capa_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
    {
        //if only p1 or p2, return nothing
        if(!p1 || !p2)
        {
            return {};
        }
        else
        {
            //return the period with same capa
            return capa_period((*p1)._capa, period);
        }
    }
};

}//namespace details
