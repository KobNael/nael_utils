#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/gregorian/greg_calendar.hpp>

#include <nael_utils/model/model_desc.hh>


namespace bo
{

class AbstractClosing
{
public:
    virtual ~AbstractClosing() = 0;

    MAKE_CLASS_ATT(
        ( (std::string)(id) ),
        ( (boost::posix_time::ptime)(from) )
        ( (boost::posix_time::ptime)(to) )
    )
    MAKE_CLASS_SORT( AbstractClosing, (from,to) )
};

class PressClosing : public AbstractClosing
{};

class ChangeOverClosing : public AbstractClosing
{};

class MoldClosing : public AbstractClosing
{};
}
