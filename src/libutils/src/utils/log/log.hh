/**
 * @file log.hh
 */
#pragma once

#include <iostream>
#include <utils/log/Logger.hh>
#include <boost/range/has_range_iterator.hpp>
#include <boost/type_traits/has_left_shift.hpp>
#include <vector>

namespace io
{

/**
 * @brief Print an object in a stream
 * @param os the ostream
 * @param obj the object
 */
template<typename T>
std::ostream &print(std::ostream &os, T const &obj)
{
    return os << obj;
}
/**
 * @brief Print a pair of object in a stream
 * @param os the ostream
 * @param p the pair
 */
template <typename U, typename V>
std::ostream &print(std::ostream &os, const std::pair<U,V> &p)
{
	os << "(";
	print(os, p.first) << ", ";
	return print(os, p.second) << ")";
}

/**
 * @brief Print every object of a range in a stream
 * @tparam Range the type of range
 * @param os the ostream
 * @param range the range of object
 */
template<typename Range>
std::ostream &printRange(std::ostream &os, Range const &range)
{
    os << "[";
    bool first{true};
    for(auto obj : range)
    {
        print(os << ((first)?"":","), obj);
        first = false;
    }
    return os << "]";
}

/**
 * @brief Print a range of object in a stream
 * @param os the ostream
 * @param range the object
 */
template <typename T>
std::ostream &print(std::ostream &os, const std::vector<T> &range)
{
	return printRange(os, range);
}

/**
 *@brief Create a singleton TeeLogger
 *@param file path to the log file
 *@return an access to the TeeLogger
 */
TeeLogger * CreateLogger(std::string const& file);
/**
 *@return an access to the TeeLogger
 */
TeeLogger * GetLogger();
/**
 *@brief Free logger memory
 */
void ClearLogger();
/**
 *@brief Set the log level
 *@param level the log level (in OFF_LVL, ERROR_LVL, WARNING_LVL, DEBUG_LVL)
 */
void SetLogLevel(LogLevel level);

}

/**
 * @brief Execute the following instruction provided that a condition is satified
 * @param cond the condition
 * @code{cpp}
 *  COND_STATEMENT(cond) instruction
 * @endcode
 * expands to
 * @code{cpp}
 *  if(!cond){} else instruction
 * @endcode
 */
#define COND_STATEMENT(cond) if(!cond) {} else

/**
 * @brief Log a message in a stream, provided that the log level is >= io::ERROR_LVL
 * @param log the log
 */
#define EROR(log) COND_STATEMENT(log->shouldLog(io::ERROR_LVL)) log->getLog()
/**
 * @brief Log a message in a stream, provided that the log level is >= io::WARNING_LVL
 * @param log the log
 */
#define WARN(log) COND_STATEMENT(log->shouldLog(io::WARNING_LVL)) log->getLog()
/**
 * @brief Log a message in a stream, provided that the log level is >= io::INFO_LVL
 * @param log the log
 */
#define INFO(log) COND_STATEMENT(log->shouldLog(io::INFO_LVL)) log->getLog()
/**
 * @brief Log a message in a stream, provided that the log level is >= io::DEBUG_LVL
 * @param log the log
 */
#define DBUG(log) COND_STATEMENT(log->shouldLog(io::DEBUG_LVL)) log->getLog()

/** @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::ERROR_LVL */
#define ERORLOG EROR(io::GetLogger())
/** @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::WARNING_LVL */
#define WARNLOG WARN(io::GetLogger())
/** @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::INFO_LVL */
#define INFOLOG INFO(io::GetLogger())
/** @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::DEBUG_LVL */
#define DBUGLOG DBUG(io::GetLogger())

/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::ERROR_LVL
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define EROR_RANGE(log, header, range) \
    COND_STATEMENT(log->shouldLog(io::ERROR_LVL)) \
        io.printRange(log->getLog() << header, range);
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::WARNING_LVL
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define WARN_RANGE(log, header, range) \
    COND_STATEMENT(log->shouldLog(io::WARNING_LVL)) \
        io.printRange(log->getLog() << header, range);
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::INFO_LVL
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define INFO_RANGE(log, header, range) \
    COND_STATEMENT(log->shouldLog(io::INFO_LVL)) \
        io.printRange(log->getLog() << header, range);
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::ERROR_LVL
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define DBUG_RANGE(log, header, range) \
    COND_STATEMENT(log->shouldLog(io::DEBUG_LVL)) \
        io.printRange(log->getLog() << header, range);

/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::ERROR_LVL
 * @param header the message
 * @param range the range
 */
#define ERORLOG_RANGE(header, range) EROR_RANGE(io::GetLogger(), header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::WARNING_LVL
 * @param header the message
 * @param range the range
 */
#define WARNLOG_RANGE(header, range) WARN_RANGE(io::GetLogger(), header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::INFO_LVL
 * @param header the message
 * @param range the range
 */
#define INFOLOG_RANGE(header, range) INFO_RANGE(io::GetLogger(), header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::DEBUG_LVL
 * @param header the message
 * @param range the range
 */
#define DBUGLOG_RANGE(header, range) DBUG_RANGE(io::GetLogger(), header, range)

/**
 * @brief print a numeric value as a percentage, with a precision of 2
 * @param val the value
 */
#define PRINT_PERCENTAGE(val) std::fixed << std::setprecision(2) << val << "%" << std::setprecision(-1)
