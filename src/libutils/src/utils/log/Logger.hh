#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>

namespace io
{
/**
 *@enum LogLevel
 *@brief Describe the verbosity of the logger
 */
enum LogLevel
{
    OFF_LVL=0,
    ERROR_LVL=1,
    WARNING_LVL=2,
    NORMAL_LVL=3,
    DEBUG_LVL=4
};

/**
 * @class AbstractLogger
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
    Logger(std::string const& file, io::LogLevel level=NORMAL_LVL)
        : _filePath(file)
        , _level(level)
        , _stream(nullptr)
    {}

    /**
     * @brief Destruct
     */
    virtual ~Logger()
    {};

    /**
     * @brief set the verbosity
     */
    void setLogLevel(io::LogLevel level)
    {
        _level = level;
    }

    /**
     * @return true if the verbosity is greater than leve
     */
    bool shouldLog(io::LogLevel level)
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

protected:
    /** @brief Path to the log file */
    std::string _filePath={""};
    /** @brief Verbosity */
    LogLevel _level={NORMAL_LVL};
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
    FileLogger(std::string const& file, io::LogLevel level=NORMAL_LVL);

    /**
     * @brief Destructor
     */
    ~FileLogger();
};

/**
 * @brief Definition of a tee
 */
typedef boost::iostreams::tee_device<std::ostream, std::ostream> TeeDevice;
typedef boost::iostreams::stream<TeeDevice> TeeStream;

/**
 * @class Logger
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
    TeeLogger(std::string const& file, io::LogLevel level=NORMAL_LVL);

    /**
     * @brief Destructor
     */
    ~TeeLogger();

private:
    /** @brief ostream to file */
    std::ofstream _fstream;
    /** @brief TeeDevice to both console and file */
    TeeDevice* _teeDevice={nullptr};
};

}