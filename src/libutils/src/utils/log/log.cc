#include "utils/log/log.hh"
#include "utils/log/Logger.hh"


namespace io
{

// Singleton
static TeeLogger *_TeeLogger={nullptr};

// Create a singleton TeeLogger
TeeLogger * CreateLogger(std::string const& file)
{
    if(nullptr == _TeeLogger)
    {
        _TeeLogger = new TeeLogger(file);
    }
    return _TeeLogger;
}
//return an access to the TeeLogger
TeeLogger * GetLogger()
{
    if(nullptr == _TeeLogger)
    {
        throw std::runtime_error("Accessing Logger before creation");
    }
    return _TeeLogger;
}
// Free logger memory
void ClearLogger()
{
    if(nullptr != _TeeLogger)
    {
        delete _TeeLogger;
        _TeeLogger=nullptr;
    }
}
// Set the log level
void SetLogLevel(LogLevel level)
{
    GetLogger()->setLogLevel(level);
}

}