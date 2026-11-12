/**
 * @file Logger.hh
 */
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/describe.hpp>

namespace io
{
    /**
     * @enum LogLevel
     * @brief Describes the verbosity of the logger
     * @see Logger::setLogLevel
     * @see io::SetLogLevel
     */
    enum class LogLevel
    {
        OFF,      ///< No log at all
        ERROR,    ///< Only Error messages
        WARNING,  ///< Error and Warning messages
        INFO,     ///< Error, Warning and Info messages
        EXTENDED, ///< Error, Warning, Info and Extended messages
        DEBUG     ///< Error, Warning, Info and Debug messages
    };
    BOOST_DESCRIBE_ENUM(LogLevel, OFF, ERROR, WARNING, INFO, EXTENDED, DEBUG)

    /**
     * @brief Print the name of a log level
     */
    inline std::ostream &operator<<(std::ostream &os, LogLevel level)
    {
        return os << boost::describe::enum_to_string(level, "UNKNOWN");
    }

    /**
     * @class Logger
     * @brief Interface for logger
     * @tparam ostream The type of output stream
     */
    template <typename ostream>
    class Logger
    {
    public:
        /**
         * @brief Constructor
         * @param file path to the log file
         * @param level initial verbosity
         */
        explicit Logger(std::string const &file, LogLevel level = io::LogLevel::INFO)
            : _filePath(file), _level(level)
        {
        }

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
         * @brief set the verbosity
         */
        LogLevel getLogLevel() const
        {
            return _level;
        }

        /**
         * @return true if the verbosity is greater than leve
         */
        bool shouldLog(LogLevel level) const
        {
            return _level >= level;
        }

        /**
         * @return the ostream (the log file is opened on the first call if not yet done)
         */
        ostream &getLog()
        {
            ensureOpen();
            return *_stream;
        };

        /**
         * @brief set the stream
         */
        void setLog(ostream &stream)
        {
            _stream = &stream;
        }

        /**
         * @brief Open the log file if not yet done
         * @remark the derived constructors open it at once, unless the level is OFF:
         *         a logger which never writes creates no file
         */
        void ensureOpen()
        {
            if (!_opened)
            {
                openFile();
                _opened = true;
            }
        }

    protected:
        /**
         * @brief Open the log file
         */
        virtual void openFile()
        {
        }

    private:
        /** @brief is the log file open */
        bool _opened = false;
        /** @brief Path to the log file */
        std::string _filePath = {""};
        /** @brief Verbosity */
        LogLevel _level = {LogLevel::INFO};
        /** @brief the stream */
        ostream *_stream = {nullptr};
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
        explicit FileLogger(std::string const &file, LogLevel level = LogLevel::INFO);

        /**
         * @brief Destructor
         */
        ~FileLogger() override;

    protected:
        void openFile() override;

    private:
        /** @brief path to the log file */
        std::string _file;
        std::unique_ptr<std::ofstream> _internal_stream = std::make_unique<std::ofstream>();
    };

    /**
     * @brief Definition of a tee
     */
    using TeeDevice = boost::iostreams::tee_device<std::ostream, std::ostream>;
    /**
     * @brief Definition of a tee stream
     */
    using TeeStream = boost::iostreams::stream<TeeDevice>;

    /**
     * @class NullBuffer
     * @brief Stream buffer discarding everything
     */
    class NullBuffer : public std::streambuf
    {
    protected:
        int_type overflow(int_type c) override
        {
            return traits_type::not_eof(c);
        }
        std::streamsize xsputn(char const *, std::streamsize n) override
        {
            return n;
        }
    };

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
         * @param console also write to the standard output
         */
        explicit TeeLogger(std::string const &file, LogLevel level = LogLevel::INFO, bool console = true);

        /**
         * @brief Destructor
         */
        ~TeeLogger() override;

    protected:
        void openFile() override;

    private:
        /** @brief path to the log file */
        std::string _file;
        /** @brief discarded output, replaces the console when disabled */
        NullBuffer _null_buffer;
        /** @brief stream on _null_buffer */
        std::ostream _null_stream{&_null_buffer};
        /** @brief ostream to file */
        std::ofstream _fstream;
        /** @brief TeeDevice to both console and file */
        TeeStream _teeStream;
    };
} // namespace io
