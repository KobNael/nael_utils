/**
 * @file period_functor.cc
 */

#include <nael_utils/period/period_functor.hh>


namespace details
{

//------------------
// struct MakeUnion
//------------------
//Compute the union between two optional periods on a given period
time_period MakeUnion::operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
{
    (void)p1;
    (void)p2;
    assert(p1 || p2);
    //if p1 or p2, return the period
    return period;
}
// Compute the union between two optional capa_periods on a given period
std::optional<capa_period> MakeUnion::operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
{
    //compute the capacity
    if(long capa( (p1?p1->_capa:0)+(p2?p2->_capa:0) ); 0 != capa)
    {
        return capa_period(capa, period);
    }
    return {};
}
// Compute the union between an optional time_period and an optional capa_period on a given period
std::optional<time_period> MakeUnion::operator()(std::optional<time_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
{
    return operator()(p1, p2?p2->_period:std::optional<time_period>({}), period);
}
//------------------
// struct MakeDiff
//------------------
// Compute the difference between two optional periods on a given period
std::optional<time_period> MakeDiff::operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
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
// Compute the difference between two optional capa_periods on a given period
std::optional<capa_period> MakeDiff::operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
{
    //compute the remaining capacity
    if(long capa( (p1?p1->_capa:0)-(p2?p2->_capa:0) ); 0 != capa)
    {
        return capa_period(capa, period);
    }
    return {};
}
// Compute the difference between an optional time_period and an optional capa_period on a given period
std::optional<time_period> MakeDiff::operator()(std::optional<time_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
{
    return operator()(p1, p2?p2->_period:std::optional<time_period>({}), period);
}
// Compute the difference between an optional capa_period and an optional time_period on a given period
std::optional<capa_period> MakeDiff::operator()(std::optional<capa_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
{
    //get the capacity of p1
    long capa ( (p1?p1->_capa:0) );
    return operator()(p1, p2?capa_period(capa, *p2):std::optional<capa_period>({}), period);
}


//------------------
// struct MakeInter
//------------------
// Compute the intersection between two optional periods on a given period
std::optional<time_period> MakeInter::operator()(std::optional<time_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
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
// Compute the intersection between two optional capa_periods on a given period
std::optional<capa_period> MakeInter::operator()(std::optional<capa_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
{
    //if only p1 or p2, return nothing
    if(!p1 || !p2)
    {
        return {};
    }
    else
    {
        if(long capa( std::min((*p1)._capa, (*p2)._capa) ); 0 != capa)
        {
            return capa_period(capa, period);
        }
        return {};
    }
}
// Compute the intersection between an optional time_period and an optional capa_period on a given period
std::optional<time_period> MakeInter::operator()(std::optional<time_period> const&p1, std::optional<capa_period> const&p2, time_period const &period) const
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
// Compute the intersection between an optional capa_period and an optional time_period on a given period
std::optional<capa_period> MakeInter::operator()(std::optional<capa_period> const&p1, std::optional<time_period> const&p2, time_period const &period) const
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

}//namespace details
