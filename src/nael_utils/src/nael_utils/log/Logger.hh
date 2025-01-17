/**
 * @file Logger.hh
 */
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>

namespace io
{
/**
 * @enum LogLevel
 * @brief Describe the verbosity of the logger
 * @see Logger::setLogLevel
 * @see io::SetLogLevel()
 */
enum class LogLevel
{
    OFF=0,      ///< No log at all
    ERROR=1,    ///< Only Error messages
    WARNING=2,  ///< Error and Warning messages
    INFO=3,     ///< Error, Warning and Info messages
    DEBUG=4     ///< Error, Warning, Info and Debug messages
};

/**
 * @class Logger
 * @brief Interface for logger
 * @tparam ostream The type of output stream
 */
template<typename ostream>
class Logger
{

public:
    /**
     * @brief Constructor
     * @param file path to the log file
     * @param level initial verbosity
     */
    Logger(std::string const& file, LogLevel level=io::LogLevel::INFO)
        : _filePath(file)
        , _level(level)
        , _stream(nullptr)
    {}

    /**
     * @brief Destruct
     */
    virtual ~Logger() = default;

    /**
     * @brief set the verbosity
     */
    void setLogLevel(LogLevel level)
    {
        _level = level;
    }

    /**
     * @return true if the verbosity is greater than leve
     */
    bool shouldLog(LogLevel level) const
    {
        return _level >= level;
    }

    /**
     * @return the ostream
     */
    ostream &getLog()
    {
        return *_stream;
    };

    /**
     * @brief set the stream
     */
    void setLog(ostream& stream)
    {
        _stream = &stream;
    }

private:
    /** @brief Path to the log file */
    std::string _filePath={""};
    /** @brief Verbosity */
    LogLevel _level={LogLevel::INFO};
    /** @brief the stream */
    ostream *_stream={nullptr};
};


/**
 * @class FileLogger
 * @brief Log in a file
 */
class FileLogger : public Logger<std::ofstream>
{
public:
    /**
     * @brief Constructor
     * @param file path to the log file
     * @param level initial verbosity
     * @throw if the file can not be open
     */
    FileLogger(std::string const& file, LogLevel level=LogLevel::INFO);

    /**
     * @brief Destructor
     */
    ~FileLogger() = default;
private:
    std::unique_ptr<std::ofstream> _internal_stream = std::make_unique<std::ofstream>();
};

/**
 * @brief Definition of a tee
 */
using TeeDevice = boost::iostreams::tee_device<std::ostream, std::ostream>;
/**
 * @brief Definition of a tee stream
 */
using TeeStream= boost::iostreams::stream<TeeDevice>;

/**
 * @class TeeLogger
 * @brief Log both in a file and a terminal
 */
class TeeLogger : public Logger<TeeStream>
{

public:
    /**
     * @brief Constructor
     * @param file path to the log file
     * @param level verbosity
     */
    TeeLogger(std::string const& file, LogLevel level=LogLevel::INFO);

    /**
     * @brief Destructor
     */
    ~TeeLogger() = default;

private:
    /** @brief ostream to file */
    std::ofstream _fstream;
    /** @brief TeeDevice to both console and file */
    std::unique_ptr<TeeDevice> _teeDevice;
    /** @brief TeeDevice to both console and file */
    std::unique_ptr<TeeStream> _teeStream;
};

}