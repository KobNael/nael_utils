/**
 * @file period_typedef.hh
 * @brief Definition of the basics periods available :
 * - #time_period : shortcut for boost::posix_time::time_period
 * - capa_period : #time_period with capacity (long long)
 * - ratio_period : #time_period with a ratio (float)
 */
#pragma once

#include <list>
#include <boost/date_time/posix_time/posix_time.hpp>

///////////
// Types //
///////////
/**
 * @typedef time_period
 * @brief Alias for time period
 * @addtogroup period_type_def
 */
using time_period = boost::posix_time::time_period;

/**
 * @interface extended_period
 * @brief Represents time_period with an additional attribute
 * @addtogroup period_type_def
 */
struct extended_period
{
public:
    /**
     * @brief constructor
     * @param period the time_period
     */
    extended_period(time_period const &period)
        : _period(period)
    {
    }
    /**
     * @brief destructor
     */
    virtual ~extended_period() = 0;

    /**
     * @brief constructor
     * @param start the starting date time
     * @param end the ending date time
     */
    extended_period(boost::posix_time::ptime const &start, boost::posix_time::ptime const &end)
        : _period(time_period(start, end))
    {
    }
    /** @return the begin of the period */
    boost::posix_time::ptime begin() const { return _period.begin(); }
    /** @return the end of the period */
    boost::posix_time::ptime end() const { return _period.end(); }
    /** @return the period duration */
    boost::posix_time::time_duration length() const { return _period.length(); }
    /** @brief Add duration to both begin and end. */
    void shift(boost::posix_time::time_duration const &d) { _period.shift(d); }
    /** @return true if the time_period intersects another time_period */
    bool intersect(extended_period const &ep) const { return _period.intersects(ep._period); }
    /** @return true if the time_period contains a ptime */
    bool contains(boost::posix_time::ptime const &t) const { return _period.contains(t); }
    /** @brief the time_period */
    time_period _period;

    /** @brief equality operator */
    bool operator==(extended_period const &ep) const = default;
};

/**
 * @struct capa_period
 * @brief Represents a capacity on a time_period
 * @addtogroup period_type_def
 */
struct capa_period : public extended_period
{
public:
    /**
     * @brief constructor
     * @param capa the capacity
     * @param period the time_period
     */
    capa_period(long long capa, time_period const &period)
        : extended_period(period), _capa(capa)
    {
    }
    /**
     * @brief constructor
     * @param capa the capacity
     * @param start the starting date time
     * @param end the ending date time
     */
    capa_period(long long capa, boost::posix_time::ptime const &start, boost::posix_time::ptime const &end)
        : extended_period(start, end), _capa(capa)
    {
    }
    /**
     * @brief destructor
     */
    ~capa_period() override = default;

    /** @brief the capacity */
    long long _capa;

    /** @brief equality operator */
    bool operator==(capa_period const &cp) const = default;

private:
    /**
     * @brief OStream operator for capa_period
     * @param os the ostream
     * @param cp the capa_period
     */
    friend std::ostream &operator<<(std::ostream &os, capa_period const &cp)
    {
        return os << "(" << cp._period << "/" << cp._capa << ")";
    }
};

/**
 * @struct ratio_period
 * @brief Represents a variation on a time_period
 * @addtogroup period_type_def
 */
struct ratio_period : public extended_period
{
public:
    /**
     * @brief constructor
     * @param ratio the variation
     * @param period the time_period
     */
    ratio_period(float ratio, time_period const &period)
        : extended_period(period), _ratio(ratio)
    {
    }
    /**
     * @brief constructor
     * @param ratio the variation
     * @param start the starting date time
     * @param end the ending date time
     */
    ratio_period(float ratio, boost::posix_time::ptime const &start, boost::posix_time::ptime const &end)
        : extended_period(start, end), _ratio(ratio)
    {
    }
    /**
     * @brief destructor
     */
    ~ratio_period() override = default;

    /**
     * @return the relative duration, ie the duration multiplied by the ratio
     */
    boost::posix_time::time_duration get_relative_duration() const;

    /** @brief the ratio */
    float _ratio;

    /** @brief equality operator */
    bool operator==(ratio_period const &cp) const = default;

private:
    /**
     * @brief OStream operator for ratio_period
     * @param os the ostream
     * @param rp the ratio_period
     */
    friend std::ostream &operator<<(std::ostream &os, ratio_period const &rp)
    {
        return os << "(" << rp._period << "/" << rp._ratio << ")";
    }
};

/**
 * @typedef LTimePeriod
 * @brief list of period
 * @addtogroup period_type_def
 */
using LTimePeriod = std::list<time_period>;
/**
 * @typedef LCapaPeriod
 * @brief list of period with capacity
 * @addtogroup period_type_def
 */
using LCapaPeriod = std::list<capa_period>;
/**
 * @typedef LRatioPeriod
 * @brief list of period with ratio
 * @addtogroup period_type_def
 */
using LRatioPeriod = std::list<ratio_period>;
