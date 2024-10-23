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
 * @param obj the object
 */
template <typename U, typename V>
std::ostream &print(std::ostream &os_p, const std::pair<U,V> &p)
{
	os_p << "(";
	print(os_p, p.first) << ", ";
	return print(os_p, p.second) << ")";
}

/**
 * @brief Print every object from a vector in a stream
 * @param os the ostream
 * @param vec the vector of object
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
std::ostream &print(std::ostream &os_p, const std::vector<T> &range)
{
	return printRange(os_p, range);
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

#define COND_STATEMENT(cond) if(!cond) {} else

#define EROR(log) COND_STATEMENT(log->shouldLog(io::ERROR_LVL)) log->getLog()
#define WARN(log) COND_STATEMENT(log->shouldLog(io::WARNING_LVL)) log->getLog()
#define INFO(log) COND_STATEMENT(log->shouldLog(io::NORMAL_LVL)) log->getLog()
#define DBUG(log) COND_STATEMENT(log->shouldLog(io::DEBUG_LVL)) log->getLog()

#define ERORLOG EROR(io::GetLogger())
#define WARNLOG WARN(io::GetLogger())
#define INFOLOG INFO(io::GetLogger())
#define DBUGLOG DBUG(io::GetLogger())

#define EROR_RANGE(log, header, range) \
    COND_STATEMENT(log->shouldLog(io::ERROR_LVL)) \
        io.printRange(log->getLog() << header, range);
#define WARN_RANGE(log, header, range) \
    COND_STATEMENT(log->shouldLog(io::WARNING_LVL)) \
        io.printRange(log->getLog() << header, range);
#define INFO_RANGE(log, header, range) \
    COND_STATEMENT(log->shouldLog(io::NORMAL_LVL)) \
        io.printRange(log->getLog() << header, range);
#define DBUG_RANGE(log, header, range) \
    COND_STATEMENT(log->shouldLog(io::DEBUG_LVL)) \
        io.printRange(log->getLog() << header, range);

#define ERORLOG_RANGE(header) EROR_RANGE(io::GetLogger(), header)
#define WARNLOG_RANGE(header) WARN_RANGE(io::GetLogger(), header)
#define INFOLOG_RANGE(header) INFO_RANGE(io::GetLogger(), header)
#define DBUGLOG_RANGE(header) DBUG_RANGE(io::GetLogger(), header)

#define PRINT_PERCENTAGE(val) std::fixed << std::setprecision(2) << val << "%" << std::setprecision(-1)
