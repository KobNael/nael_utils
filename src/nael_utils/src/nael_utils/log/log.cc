/**
 * @file log.cc
 */
#include <nael_utils/log/log.hh>

namespace io
{

std::unordered_map<std::string, TeeLogger> LoggerManager::_loggers;
std::string LoggerManager::_logfile_name = std::string("default");

TeeLogger &LoggerManager::GetLogger(std::string const &name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    _loggers.try_emplace(act_name, act_name + ".log");
    return _loggers.at(act_name);
}

void LoggerManager::ClearLogger(std::string const &name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    _loggers.erase(act_name);
}

void LoggerManager::SetLogLevel(LogLevel level, std::string const&name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    GetLogger(act_name).setLogLevel(level);
}

}