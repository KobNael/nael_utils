/**
 * @file Logger.cc
 */
#include <nael_utils/log/Logger.hh>
#include <nael_utils/exception/exception.hh>

#include <filesystem>
#include <format>

namespace io
{

    namespace
    {
        /**
         * @brief Open a stream
         */
        void open_stream(std::string const &file, std::ofstream &stream)
        {
            if (file != "")
            {
                stream.open(file.c_str(), std::ios::out);
                if (stream.fail())
                {
                    throw io::access_error(std::format("Could not create log file {}", file));
                }
                else
                {
                    stream.imbue(std::locale(std::locale::classic()));
                }
            }
            else
            {
                throw io::access_error("Invalid name of log file");
            }
        }

        /**
         * @brief Open the log file of a logger, unless its level is OFF: the file is then removed
         *        (so that no stale log remains) and only created on the first write
         */
        template <typename LoggerT>
        void open_or_remove(LoggerT &logger, std::string const &file, io::LogLevel level)
        {
            if (level != io::LogLevel::OFF)
            {
                logger.ensureOpen();
            }
            else
            {
                std::error_code ignored;
                std::filesystem::remove(file, ignored);
            }
        }
    } // namespace

    // FileLogger constructor
    FileLogger::FileLogger(std::string const &file, io::LogLevel level)
        : Logger<std::ofstream>(file, level), _file(file)
    {
        this->setLog(*_internal_stream);
        open_or_remove(*this, file, level);
    }
    void FileLogger::openFile()
    {
        open_stream(_file, *_internal_stream);
    }
    FileLogger::~FileLogger()
    {
        if (_internal_stream->is_open())
        {
            _internal_stream->flush();
            _internal_stream->close();
        }
    }

    // TeeLogger constructor
    TeeLogger::TeeLogger(std::string const &file, io::LogLevel level, bool console)
        : Logger<TeeStream>(file, level), _file(file), _teeStream(TeeDevice(console ? std::cout : _null_stream, _fstream))
    {
        this->setLog(_teeStream);
        open_or_remove(*this, file, level);
    }
    void TeeLogger::openFile()
    {
        open_stream(_file, _fstream);
    }
    TeeLogger::~TeeLogger()
    {
        if (_fstream.is_open())
        {
            _fstream.flush();
            _fstream.close();
        }
    }

} // namespace io