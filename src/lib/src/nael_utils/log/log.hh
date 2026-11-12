/**
 *@file log.hh
 *@brief log utilities
 */

#pragma once

#include <nael_utils/log/Logger.hh>
#include <nael_utils/exception/exception.hh>
#include <nael_utils/hash/hash.hh>

#include <filesystem>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <functional>
#include <boost/range/has_range_iterator.hpp>
#include <boost/type_traits/has_left_shift.hpp>

namespace io
{

    /**
     * @struct LogState
     * @brief Loggers and settings used by the LoggerManager
     */
    struct LogState
    {
        /** @brief tee loggers by name */
        std::unordered_map<std::string, TeeLogger, string_hash, std::equal_to<>> tee_loggers;
        /** @brief file loggers by name */
        std::unordered_map<std::string, FileLogger, string_hash, std::equal_to<>> file_loggers;
        /** @brief default log file name */
        std::string logfile_name = "default";
        /** @brief default log level */
        LogLevel log_level = LogLevel::INFO;
        /** @brief directory of the log files (current directory if empty) */
        std::filesystem::path directory;
        /** @brief do the tee loggers also write to the standard output */
        bool console = true;
        /** @brief protects the loggers maps (threads of a pool share the state of their session) */
        std::mutex mutex;

        /** @return the path of the log file of a logger */
        std::string file_path(std::string_view name) const;
    };

    /**
     * @class LogSession
     * @brief Give the current thread its own loggers and settings for its lifetime
     *
     * The LoggerManager uses a process wide LogState, unless a LogSession is alive on the calling thread.
     * The loggers created during the session are closed when it ends.
     * Sessions on different threads are independent, so several runs may log concurrently
     * (in different directories, as the log files are named after the loggers).
     * The jobs of a thread::Pool use the session of the thread which enqueued them;
     * other threads may join a session with a LogSessionScope.
     * @warning the session must outlive the threads using it
     */
    class LogSession
    {
    public:
        /**
         * @brief Start a session on the current thread
         * @param directory directory of the log files (current directory if empty), created if needed
         * @param level default log level
         * @param name default log file name
         * @param console do the tee loggers also write to the standard output
         */
        LogSession(std::filesystem::path directory, LogLevel level, std::string name, bool console);
        /**
         * @brief End the session: close its loggers, restore the previous state of the thread
         */
        ~LogSession();

        LogSession(LogSession const &) = delete;
        LogSession &operator=(LogSession const &) = delete;

    private:
        friend class LoggerManager;
        /** @brief loggers and settings of the session */
        LogState _state;
        /** @brief state of the thread before the session */
        LogState *_previous;
    };

    /**
     * @class LogSessionScope
     * @brief Make the current thread use the state of a session (of another thread) for its lifetime
     *
     * @code{cpp}
     * io::LogState *session = io::LoggerManager::CurrentSession();
     * std::thread worker([session]() {
     *     io::LogSessionScope scope(session);
     *     INFOLOG << "logged in the session of the parent thread" << std::endl;
     * });
     * @endcode
     */
    class LogSessionScope
    {
    public:
        /**
         * @param state the state of the session (from LoggerManager::CurrentSession()), null for the process wide state
         */
        explicit LogSessionScope(LogState *state);
        /** @brief Restore the previous state of the thread */
        ~LogSessionScope();

        LogSessionScope(LogSessionScope const &) = delete;
        LogSessionScope &operator=(LogSessionScope const &) = delete;

    private:
        /** @brief state of the thread before the scope */
        LogState *_previous;
    };

    /**
     * @class LoggerManager
     * @brief Store and give access to TeeLoggers
     */
    class LoggerManager
    {
    public:
        // TeeLoggers
        /**
         * @brief Get or create a TeeLogger
         * @param name name of the logger, empty string for default name
         * @return A reference to the logger
         */
        static TeeLogger &GetLogger(std::string_view name = "");
        /**
         * @brief Clear a logger
         * @param name name of the logger, empty string for default name
         */
        static void ClearLogger(std::string_view name = "");
        /**
         * @brief Set the loglevel of a TeeLogger
         * @param name name of the logger, empty string for default name
         * @param level the log level
         */
        static void SetLogLevel(LogLevel level, std::string_view name = "");
        /**
         * @brief Set the default verbosity
         * @param level the verbosity
         */
        static void SetDefaultLogLevel(LogLevel level)
        {
            state().log_level = level;
        }
        /**
         * @brief Set the default name
         * @param name name of the logger
         */
        static void SetDefaultName(std::string_view name)
        {
            state().logfile_name = name;
        }

        /**
         * @return the state of the session of the current thread, null if none (process wide state)
         * @see LogSessionScope
         */
        static LogState *CurrentSession()
        {
            return _session_state;
        }

    private:
        friend class LogSession;
        friend class LogSessionScope;
        /** @return the state of the session of the current thread, the process wide state if none */
        static LogState &state();
        /** @brief process wide state */
        static LogState _global_state;
        /** @brief state of the session of the current thread (null if none) */
        static thread_local LogState *_session_state;

        // FileLoggers
    public:
        /**
         * @brief Get or create a FileLogger
         * @param name name of the logger
         * @return A reference to the logger
         */
        static FileLogger &GetFileLogger(std::string_view name);
        /**
         * @brief Clear a FileLogger
         * @param name name of the logger
         */
        static void ClearFileLogger(std::string_view name);
        /**
         * @brief Set the loglevel of a FileLogger
         * @param name name of the logger
         * @param level the log level
         */
        static void SetFileLogLevel(LogLevel level, std::string_view name);

        /**
         * @brief Clear every logger (file or tee)
         */
        static void ClearAllLoggers();
    };

    /*
     * Forward declare of print_range.
     */
    template <typename Range>
    std::ostream &print_range(std::ostream &os, Range const &range);

    /**
     * @brief Print an object in a stream
     * @tparam T the type of object
     * @param os the ostream
     * @param obj the object
     */
    template <typename T>
    std::ostream &print(std::ostream &os, T const &obj)
    {
        return os << obj;
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
        return print_range(os, range);
    }

    /**
     * @brief Print an object in a stream
     * @tparam T the type of object
     * @param os the ostream
     * @param obj the object
     */
    template <typename T>
    std::ostream &print(std::ostream &os, std::reference_wrapper<T> const &obj)
    {
        return print(os, obj.get());
    }
    /**
     * @brief Print a pair of object in a stream
     * @tparam U the first type of object in the pair
     * @tparam V the second type of object in the pair
     * @param os the ostream
     * @param p the pair
     */
    template <typename U, typename V>
    std::ostream &print(std::ostream &os, const std::pair<U, V> &p)
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
    template <typename Range>
    std::ostream &print_range(std::ostream &os, Range const &range)
    {
        os << "[";
        bool first{true};
        for (auto obj : range)
        {
            print(os << (first ? "" : ","), obj);
            first = false;
        }
        return os << "]";
    }

} // namespace io

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
#define COND_STATEMENT(cond) \
    if (!cond)               \
    {                        \
    }                        \
    else

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
 * @brief Log a message in a stream, provided that the log level is >= io::LogLevel::EXTENDED
 * @param log the log
 */
#define EXTD(log) COND_STATEMENT(log.shouldLog(io::LogLevel::EXTENDED)) log.getLog()
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
 * @brief Log a message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::EXTENDED
 * @throw if the main io::TeeLogger has not been created
 */
#define EXTDLOG EXTD(io::LoggerManager::GetLogger())
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
#define EROR_RANGE(log, header, range)                 \
    COND_STATEMENT(log.shouldLog(io::LogLevel::ERROR)) \
    io::print_range(log.getLog() << header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::WARNING
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define WARN_RANGE(log, header, range)                   \
    COND_STATEMENT(log.shouldLog(io::LogLevel::WARNING)) \
    io::print_range(log.getLog() << header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::INFO
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define INFO_RANGE(log, header, range)                \
    COND_STATEMENT(log.shouldLog(io::LogLevel::INFO)) \
    io::print_range(log.getLog() << header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::EXTENDED
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define EXTD_RANGE(log, header, range)                    \
    COND_STATEMENT(log.shouldLog(io::LogLevel::EXTENDED)) \
    io::print_range(log.getLog() << header, range)
/**
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::DEBUG
 * @param log the log
 * @param header the message
 * @param range the range
 */
#define DBUG_RANGE(log, header, range)                 \
    COND_STATEMENT(log.shouldLog(io::LogLevel::DEBUG)) \
    io::print_range(log.getLog() << header, range)

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
 * @brief Log a range with a header message in the main io::TeeLogger, provided that the log level is >= io::LogLevel::EXTENDED
 * @param header the message
 * @param range the range
 * @throw if the main io::TeeLogger has not been created
 */
#define EXTDLOG_RANGE(header, range) EXTD_RANGE(io::LoggerManager::GetLogger(), header, range)
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

/**
 * @brief print an enum value created with MAKE_DTO_ENUM as a string
 * @param val the value
 */
#define PRINT_ENUM(val) boost::describe::enum_to_string(val, "unkown")
