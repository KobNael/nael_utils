/**
 * @file log.cc
 */
#include <nael_utils/log/log.hh>

namespace io
{

std::unordered_map<std::string, TeeLogger, std::hash<std::string_view>, std::equal_to<>> LoggerManager::_loggers;
std::string LoggerManager::_logfile_name = std::string("default");

TeeLogger &LoggerManager::GetLogger(std::string_view name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    _loggers.try_emplace(act_name, act_name + ".log");
    return _loggers.at(act_name);
}

void LoggerManager::ClearLogger(std::string_view name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    _loggers.erase(act_name);
}

void LoggerManager::SetLogLevel(LogLevel level, std::string_view name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    GetLogger(act_name).setLogLevel(level);
}

}