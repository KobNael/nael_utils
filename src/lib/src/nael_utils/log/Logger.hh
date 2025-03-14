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
     * @see io::SetLogLevel()
     */
    BOOST_DEFINE_ENUM(
        LogLevel,
        OFF,      ///< No log at all
        ERROR,    ///< Only Error messages
        WARNING,  ///< Error and Warning messages
        INFO,     ///< Error, Warning and Info messages
        EXTENDED, ///< Error, Warning, Info and Extended messages
        DEBUG     ///< Error, Warning, Info and Debug messages
    );

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

        protected:
            /** @brief Path to the log file */
            std::string _filePath={""};
        private:
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
            ~FileLogger() override;
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
            ~TeeLogger() override;

        private:
            /** @brief ostream to file */
            std::ofstream _fstream;
            /** @brief TeeDevice to both console and file */
            std::unique_ptr<TeeDevice> _teeDevice;
            /** @brief TeeDevice to both console and file */
            std::unique_ptr<TeeStream> _teeStream;
    };
}//namespace io
