/**
 * @file Logger.cc
 */
#include <nael_utils/log/Logger.hh>
#include <nael_utils/exception/exception.hh>

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
    } // namespace

    // FileLogger constructor
    FileLogger::FileLogger(std::string const &file, io::LogLevel level)
        : Logger<std::ofstream>(file, level)
    {
        open_stream(file, *_internal_stream);
        this->setLog(*_internal_stream);
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
    TeeLogger::TeeLogger(std::string const &file, io::LogLevel level)
        : Logger<TeeStream>(file, level), _teeStream(TeeDevice(std::cout, _fstream))
    {
        open_stream(file, _fstream);
        this->setLog(_teeStream);
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