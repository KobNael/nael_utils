/**
 *@file log.hh
 *@brief log utilities
 */

#pragma once

#include <nael_utils/log/Logger.hh>
#include <nael_utils/exception/exception.hh>
#include <nael_utils/hash/hash.hh>

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <boost/range/has_range_iterator.hpp>
#include <boost/type_traits/has_left_shift.hpp>

namespace io
{

/**
 * @class LoggerManager
 * @brief Store and give access to TeeLoggers
 */
class LoggerManager
{
public:
    /**
     * @brief Get or create a TeeLogger
     * @param name name of the logger, empty string for default name
     * @return A reference to the logger
     */
    static TeeLogger &GetLogger(std::string_view name="");
    /**
     * @brief Clear a looger
     * @param name name of the logger, empty string for default name
     */
    static void ClearLogger(std::string_view name="");
    /**
     * @brief Set the loglevel of a TeeLogger
     * @param name name of the logger, empty string for default name
     * @param level the log level
     */
    static void SetLogLevel(LogLevel level, std::string_view name="");
    /**
     * @brief Set the default name
     * @param name name of the logger
     */
    static void SetDefaultName(std::string_view name)
    {
        _logfile_name = name;
    }

private:
    /** @brief static storage of the logger */
    static std::unordered_map<std::string, TeeLogger, string_hash, std::equal_to<>> _loggers;
    /** @brief default log file name */
    static std::string _logfile_name;
};

/**
 * @brief Set the log level
 * @param level the log level
 */
void SetLogLevel(LogLevel level);

/**
 * @brief Print an object in a stream
 * @tparam T the type of object
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
 * @tparam U the first type of object in the pair
 * @tparam V the second type of object in the pair
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
        print(os << (first?"":","), obj);
        first = false;
    }
    return os << "]";
}

/**
 * @brief Print a range of object in a stream
 * @tparam T the type of object
 * @param os the ostream
 * @param range the object
 */
template <typename T>
std::ostream &print(std::ostream &os, const std::vector<T> &range)
{
	return printRange(os, range);
}

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
 * @brief Log a message in a stream, provided that the log level is >= io::LogLevel::ERROR
 * @param log the log
 */
#define EROR(log) COND_STATEMENT(log.shouldLog(io::LogLevel::ERROR)) log.getLog()
/**
 * @brief Log a message in a stream, provided that the log level is >= io::LogLevel::WARNING
 * @param log the log
 */
#define WARN(log) COND_STATEMENT(log.shouldLog(io::LogLevel::WARNING)) log.getLog()
/**
 * @brief Log a message in a stream, provided that the log level is >= io::LogLevel::INFO
 * @param log the log
 */
#define INFO(log) COND_STATEMENT(log.shouldLog(io::LogLevel::INFO)) log.getLog()
/**
 * @brief Log a message in a stream, provided that the log level is >= io::LogLevel::DEBUG
 * @param log the log
 */
#define DBUG(log) COND_STATEMENT(log.shouldLog(io::LogLevel::DEBUG)) log.getLog()

/**
 * @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::ERROR
 * @throw if the main io::TeeLogger has not been created
 */
#define ERORLOG EROR(io::LoggerManager::GetLogger())
/**
 * @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::WARNING
 * @throw if the main io::TeeLogger has not been created
 */
#define WARNLOG WARN(io::LoggerManager::GetLogger())
/**
 * @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::INFO
 * @throw if the main io::TeeLogger has not been created
 */
#define INFOLOG INFO(io::LoggerManager::GetLogger())
/**
 * @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::DEBUG
 * @throw if the main io::TeeLogger has not been created
 */
#define DBUGLOG DBUG(io::LoggerManager::GetLogger())

/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::ERROR
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define EROR_RANGE(log, header, range) \
    COND_STATEMENT(log.shouldLog(io::LogLevel::ERROR)) \
        io::printRange(log.getLog() << header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::WARNING
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define WARN_RANGE(log, header, range) \
    COND_STATEMENT(log.shouldLog(io::LogLevel::WARNING)) \
        io::printRange(log.getLog() << header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::INFO
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define INFO_RANGE(log, header, range) \
    COND_STATEMENT(log.shouldLog(io::LogLevel::INFO)) \
        io::printRange(log.getLog() << header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::DEBUG
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define DBUG_RANGE(log, header, range) \
    COND_STATEMENT(log.shouldLog(io::LogLevel::DEBUG)) \
        io::printRange(log.getLog() << header, range)

/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::ERROR
 * @param header the message
 * @param range the range
 * @throw if the main io::TeeLogger has not been created
 */
#define ERORLOG_RANGE(header, range) EROR_RANGE(io::LoggerManager::GetLogger(), header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::WARNING
 * @param header the message
 * @param range the range
 * @throw if the main io::TeeLogger has not been created
 */
#define WARNLOG_RANGE(header, range) WARN_RANGE(io::LoggerManager::GetLogger(), header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::INFO
 * @param header the message
 * @param range the range
 * @throw if the main io::TeeLogger has not been created
 */
#define INFOLOG_RANGE(header, range) INFO_RANGE(io::LoggerManager::GetLogger(), header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::DEBUG
 * @param header the message
 * @param range the range
 * @throw if the main io::TeeLogger has not been created
 */
#define DBUGLOG_RANGE(header, range) DBUG_RANGE(io::LoggerManager::GetLogger(), header, range)

/**
 * @brief print a numeric value as a percentage, with a precision of 2
 * @param val the value
 */
#define PRINT_PERCENTAGE(val) std::fixed << std::setprecision(2) << val << "%" << std::setprecision(-1)
