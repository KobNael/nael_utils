/**
 * @file log.cc
 */
#include <nael_utils/log/log.hh>

namespace io
{

    std::string LogState::file_path(std::string_view name) const
    {
        std::string file_name = std::string(name) + ".log";
        return directory.empty() ? file_name : (directory / file_name).string();
    }

    LogSession::LogSession(std::filesystem::path directory, LogLevel level, std::string name, bool console)
        : _previous(LoggerManager::_session_state)
    {
        if (!directory.empty())
        {
            std::filesystem::create_directories(directory);
        }
        _state.directory = std::move(directory);
        _state.log_level = level;
        _state.logfile_name = std::move(name);
        _state.console = console;
        LoggerManager::_session_state = &_state;
    }

    LogSession::~LogSession()
    {
        LoggerManager::_session_state = _previous;
    }

    LogSessionScope::LogSessionScope(LogState *state)
        : _previous(LoggerManager::_session_state)
    {
        LoggerManager::_session_state = state;
    }

    LogSessionScope::~LogSessionScope()
    {
        LoggerManager::_session_state = _previous;
    }

    LogState LoggerManager::_global_state;
    thread_local LogState *LoggerManager::_session_state = nullptr;

    LogState &LoggerManager::state()
    {
        return _session_state ? *_session_state : _global_state;
    }

    TeeLogger &LoggerManager::GetLogger(std::string_view name)
    {
        LogState &st = state();
        std::lock_guard lock(st.mutex);
        std::string act_name{(name.empty() ? st.logfile_name : name)};
        auto iter = st.tee_loggers.find(act_name);
        if (iter == st.tee_loggers.end())
        {
            iter = st.tee_loggers.try_emplace(act_name, st.file_path(act_name), st.log_level, st.console).first;
        }
        return iter->second;
    }

    void LoggerManager::ClearLogger(std::string_view name)
    {
        LogState &st = state();
        std::lock_guard lock(st.mutex);
        std::string act_name{(name.empty() ? st.logfile_name : name)};
        st.tee_loggers.erase(act_name);
    }

    void LoggerManager::SetLogLevel(LogLevel level, std::string_view name)
    {
        GetLogger(name).setLogLevel(level);
    }

    FileLogger &LoggerManager::GetFileLogger(std::string_view name)
    {
        LogState &st = state();
        std::lock_guard lock(st.mutex);
        auto iter = st.file_loggers.find(name);
        if (iter == st.file_loggers.end())
        {
            iter = st.file_loggers.try_emplace(std::string(name), st.file_path(name), st.log_level).first;
        }
        return iter->second;
    }

    void LoggerManager::ClearFileLogger(std::string_view name)
    {
        LogState &st = state();
        std::lock_guard lock(st.mutex);
        st.file_loggers.erase(std::string(name));
    }

    void LoggerManager::SetFileLogLevel(LogLevel level, std::string_view name)
    {
        GetFileLogger(name).setLogLevel(level);
    }

    void LoggerManager::ClearAllLoggers()
    {
        LogState &st = state();
        std::lock_guard lock(st.mutex);
        st.tee_loggers.clear();
        st.file_loggers.clear();
    }

}
