/**
 * @file period_functor.cc
 */

#include <nael_utils/period/period_functor.hh>

#include <nael_utils/safe_comp/safe_comp.hh>


extern time_period make_empty_period(boost::posix_time::ptime ptime);
extern bool is_same(boost::posix_time::ptime const &lhs, boost::posix_time::ptime const &rhs, boost::posix_time::time_duration const &tol=boost::posix_time::milliseconds(10));

namespace details
{

    //------------------
    // struct MakeUnion
    //------------------
    //Compute the union between two optional periods on a given period
    time_period MakeUnion::operator()(std::optional<time_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        (void)p1;
        (void)p2;
        assert(p1 || p2);
        //if p1 or p2, return the period
        return time_period(from, to);
    }
    // Compute the union between two optional capa_period(from, to)s on a given period
    std::optional<capa_period> MakeUnion::operator()(std::optional<capa_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //compute the capacity
        if(long capa( (p1?p1->_capa:0)+(p2?p2->_capa:0) ); 0 != capa)
        {
            return capa_period(capa, time_period(from, to));
        }
        return {};
    }
    // Compute the union between two optional ratio_period(from, to)s on a given period
    std::optional<ratio_period> MakeUnion::operator()(std::optional<ratio_period> const &p1, std::optional<ratio_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        float ratio_1(p1?p1->_ratio:1.);
        float ratio_2(p2?p2->_ratio:1.);
        return ratio_period(ratio_1*ratio_2, time_period(from, to));
    }
    // Compute the union between an optional time_period and an optional capa_period on a given period
    std::optional<time_period> MakeUnion::operator()(std::optional<time_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        return operator()(p1, p2?p2->_period:std::optional<time_period>({}), from, to);
    }

    //------------------
    // struct MakeDiff
    //------------------
    // Compute the difference between two optional periods on a given period
    std::optional<time_period> MakeDiff::operator()(std::optional<time_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //if !p1 or both periods return nothing
        if(!p1 || p2)
        {
            return {};
        }
        //if only p1 return the full period
        else
        {
            return time_period(from, to);
        }
    }
    // Compute the difference between two optional capa_periods on a given period
    std::optional<capa_period> MakeDiff::operator()(std::optional<capa_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //compute the remaining capacity
        if(long capa( (p1?p1->_capa:0)-(p2?p2->_capa:0) ); 0 != capa)
        {
            return capa_period(capa, time_period(from, to));
        }
        return {};
    }
    // Compute the difference between an optional time_period and an optional capa_period on a given period
    std::optional<time_period> MakeDiff::operator()(std::optional<time_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        return operator()(p1, p2?p2->_period:std::optional<time_period>({}), from, to);
    }
    // Compute the difference between an optional capa_period and an optional time_period on a given period
    std::optional<capa_period> MakeDiff::operator()(std::optional<capa_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //get the capacity of p1
        long capa ( (p1?p1->_capa:0) );
        return operator()(p1, p2?capa_period(capa, *p2):std::optional<capa_period>({}), from, to);
    }

    //------------------
    // struct MakeInter
    //------------------
    // Compute the intersection between two optional periods on a given period
    std::optional<time_period> MakeInter::operator()(std::optional<time_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //if both periods
        if(p1 && p2)
        {
            if(is_same(from, to) && _keep_empty)
            {
                return make_empty_period(from);
            }
            else if(!is_same(from, to))
            {
                return time_period(from, to);
            }
        }
        return {};
    }
    // Compute the intersection between two optional capa_periods on a given period
    std::optional<capa_period> MakeInter::operator()(std::optional<capa_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //if both periods
        if(p1 && p2)
        {
            //If there is a valid capacity
            if(long capa( std::min((*p1)._capa, (*p2)._capa) ); 0 != capa)
            {
                //Get the intersection of the period and return
                if(auto inter = operator()((*p1)._period, (*p2)._period, from, to); inter)
                {
                    return capa_period(capa, *inter);
                }
            }
        }
        return {};
    }
    // Compute the intersection between an optional time_period and an optional capa_period on a given period
    std::optional<time_period> MakeInter::operator()(std::optional<time_period> const &p1, std::optional<capa_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //if both periods
        if(p1 && p2)
        {
            //make the intersection on corresponding periods
            return operator()(*p1, (*p2)._period, from, to);
        }
        return {};
    }
    // Compute the intersection between an optional capa_period and an optional time_period on a given period
    std::optional<capa_period> MakeInter::operator()(std::optional<capa_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //if both periods
        if(p1 && p2)
        {
            //check intersection validity
            if(auto inter = operator()((*p1)._period, (*p2), from, to); inter)
            {
                return capa_period((*p1)._capa, *inter);
            }
        }
        return {};
    }
    // Compute the intersection between an optional ratio_period and an optional time_period on a given period
    std::optional<ratio_period> MakeInter::operator()(std::optional<ratio_period> const &p1, std::optional<time_period> const &p2, boost::posix_time::ptime const &from, boost::posix_time::ptime const &to) const
    {
        //If not ratio is given, default value is 1., so just check that p2 is here
        if(p2)
        {
            if(float ratio( p1?p1->_ratio:1.); !safecomp::eq<float>(ratio, 0.))
            {
                //Get the intersection just on p2
                if(auto inter = operator()((*p2), (*p2), from, to); inter)
                {
                    return ratio_period(ratio, *inter);
                }
            }
        }
        return {};
    }


}//namespace details
