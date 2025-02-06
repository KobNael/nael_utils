/**
 * @file log.cc
 */
#include <nael_utils/log/log.hh>

namespace io
{

std::unordered_map<std::string, TeeLogger, string_hash, std::equal_to<>> LoggerManager::_tee_loggers;
LogLevel LoggerManager::_log_level = LogLevel::INFO;
std::string LoggerManager::_logfile_name = std::string("default");


TeeLogger &LoggerManager::GetLogger(std::string_view name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    _tee_loggers.try_emplace(act_name, act_name + ".log", LoggerManager::_log_level);
    return _tee_loggers.at(act_name);
}

void LoggerManager::ClearLogger(std::string_view name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    _tee_loggers.erase(act_name);
}

void LoggerManager::SetLogLevel(LogLevel level, std::string_view name)
{
    std::string act_name{(name.empty()?_logfile_name:name)};
    GetLogger(act_name).setLogLevel(level);
}


std::unordered_map<std::string, FileLogger, string_hash, std::equal_to<>> LoggerManager::_file_loggers;

FileLogger &LoggerManager::GetFileLogger(std::string_view name)
{
    _file_loggers.try_emplace(std::string(name), std::string(name) + ".log", LoggerManager::_log_level);
    return _file_loggers.at(std::string(name));
}

void LoggerManager::ClearFileLogger(std::string_view name)
{
    _file_loggers.erase(std::string(name));
}

void LoggerManager::SetFileLogLevel(LogLevel level, std::string_view name)
{
    GetFileLogger(name).setLogLevel(level);
}

}