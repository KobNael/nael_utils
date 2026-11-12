#include <gtest/gtest.h>

#include <nael_utils/log/log.hh>
#include <nael_utils/thread/pool.hh>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>

#define LOG_NAME "test_logger"
#define LOGFILE_NAME "test_logger.log"

//Tools for tests
namespace{
    //Parse the log file
    std::string get_log_content()
    {
        std::ostringstream log_content;
        // Parse line by line log file
        std::ifstream log(LOGFILE_NAME);
        if (!log.is_open())
        {
            // a logger which never writes creates no file
            return "";
        }
        std::string line;
        while (getline(log, line))
        {
            log_content << line << std::endl;
        }
        log.close();
        //return content
        return log_content.str();
    }

    //check string content
    bool check_nb_line(std::string pattern, std::string content, unsigned ref)
    {
        unsigned nb = 0;
        std::string::size_type pos = 0;
        while ((pos = content.find(pattern, pos )) != std::string::npos) {
                ++ nb;
                pos += pattern.length();
        }
        return nb == ref;
   }
} //namespace

/**
 * LogLevel order
 */

TEST(log_level, ordering)
{
    ASSERT_TRUE(io::LogLevel::OFF >= io::LogLevel::OFF);
    ASSERT_TRUE(io::LogLevel::OFF < io::LogLevel::ERROR);
    ASSERT_TRUE(io::LogLevel::OFF < io::LogLevel::WARNING);
    ASSERT_TRUE(io::LogLevel::OFF < io::LogLevel::INFO);
    ASSERT_TRUE(io::LogLevel::OFF < io::LogLevel::DEBUG);

    ASSERT_TRUE(io::LogLevel::ERROR >= io::LogLevel::OFF);
    ASSERT_TRUE(io::LogLevel::ERROR >= io::LogLevel::ERROR);
    ASSERT_TRUE(io::LogLevel::ERROR < io::LogLevel::WARNING);
    ASSERT_TRUE(io::LogLevel::ERROR < io::LogLevel::INFO);
    ASSERT_TRUE(io::LogLevel::ERROR < io::LogLevel::DEBUG);

    ASSERT_TRUE(io::LogLevel::WARNING >= io::LogLevel::OFF);
    ASSERT_TRUE(io::LogLevel::WARNING >= io::LogLevel::ERROR);
    ASSERT_TRUE(io::LogLevel::WARNING >= io::LogLevel::WARNING);
    ASSERT_TRUE(io::LogLevel::WARNING < io::LogLevel::INFO);
    ASSERT_TRUE(io::LogLevel::WARNING < io::LogLevel::DEBUG);

    ASSERT_TRUE(io::LogLevel::INFO >= io::LogLevel::OFF);
    ASSERT_TRUE(io::LogLevel::INFO >= io::LogLevel::ERROR);
    ASSERT_TRUE(io::LogLevel::INFO >= io::LogLevel::WARNING);
    ASSERT_TRUE(io::LogLevel::INFO >= io::LogLevel::INFO);
    ASSERT_TRUE(io::LogLevel::INFO < io::LogLevel::DEBUG);

    ASSERT_TRUE(io::LogLevel::DEBUG >= io::LogLevel::OFF);
    ASSERT_TRUE(io::LogLevel::DEBUG >= io::LogLevel::ERROR);
    ASSERT_TRUE(io::LogLevel::DEBUG >= io::LogLevel::WARNING);
    ASSERT_TRUE(io::LogLevel::DEBUG >= io::LogLevel::INFO);
    ASSERT_TRUE(io::LogLevel::DEBUG >= io::LogLevel::DEBUG);
}

/**
 * FileLogger usage
 */
class file_logger: public ::testing::Test {
protected:
    //Access to logger
    io::FileLogger &get_logger()
    {
        return io::LoggerManager::GetFileLogger(LOG_NAME);
    }
    void TearDown() override
    {
        io::LoggerManager::ClearFileLogger(LOG_NAME);
    }
};

TEST_F(file_logger, off_level)
{
    //set log level
    get_logger().setLogLevel(io::LogLevel::OFF);
    //Send the content
    EROR(get_logger()) << "ERORLOG_print" << std::endl;
    WARN(get_logger()) << "WARNLOG_print" << std::endl;
    INFO(get_logger()) << "INFOLOG_print" << std::endl;
    DBUG(get_logger()) << "DBUGLOG_print" << std::endl;
    //Close stream
    get_logger().getLog().close();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 0));
}

TEST_F(file_logger, error_lvl)
{
    //set log level
    get_logger().setLogLevel(io::LogLevel::ERROR);
    //Send the content
    EROR(get_logger()) << "ERORLOG_print" << std::endl;
    WARN(get_logger()) << "WARNLOG_print" << std::endl;
    INFO(get_logger()) << "INFOLOG_print" << std::endl;
    DBUG(get_logger()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 1));
}
TEST_F(file_logger, warning_lvl)
{
    //set log level
    get_logger().setLogLevel(io::LogLevel::WARNING);
    //Send the content
    EROR(get_logger()) << "ERORLOG_print" << std::endl;
    WARN(get_logger()) << "WARNLOG_print" << std::endl;
    INFO(get_logger()) << "INFOLOG_print" << std::endl;
    DBUG(get_logger()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 2));
}
TEST_F(file_logger, info_lvl)
{
    //set log level
    get_logger().setLogLevel(io::LogLevel::INFO);
    //Send the content
    EROR(get_logger()) << "ERORLOG_print" << std::endl;
    WARN(get_logger()) << "WARNLOG_print" << std::endl;
    INFO(get_logger()) << "INFOLOG_print" << std::endl;
    DBUG(get_logger()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 3));
}
TEST_F(file_logger, debug_lvl)
{
    //set log level
    get_logger().setLogLevel(io::LogLevel::DEBUG);
    //Send the content
    EROR(get_logger()) << "ERORLOG_print" << std::endl;
    WARN(get_logger()) << "WARNLOG_print" << std::endl;
    INFO(get_logger()) << "INFOLOG_print" << std::endl;
    DBUG(get_logger()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("_print", log_content, 4));
}

/**
 * TeeLogger usage
 */
class tee_logger: public ::testing::Test {
protected:
    void SetUp() override
    {
        io::LoggerManager::SetDefaultName(LOG_NAME);
        _bckup = std::cout.rdbuf();
        _cout.str("");
        _cout.clear();
        std::cout.rdbuf(_cout.rdbuf());
    }
    void TearDown() override
    {
        std::cout.rdbuf(_bckup);
        io::LoggerManager::ClearLogger();
    }
    std::string get_cout()
    {
        return _cout.str();
    }
private:
    std::stringstream _cout;
    std::streambuf *_bckup;
};

TEST_F(tee_logger, off_level)
{
    //set log level
    io::LoggerManager::SetLogLevel(io::LogLevel::OFF);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    EXTDLOG << "EXTDLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = get_cout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 0));
    //Test console
    EXPECT_TRUE(check_nb_line("ERORLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("_print", cout_content, 0));
}

TEST_F(tee_logger, error_lvl)
{
    //set log level
    io::LoggerManager::SetLogLevel(io::LogLevel::ERROR);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    EXTDLOG << "EXTDLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = get_cout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 1));
    //Test console
    EXPECT_TRUE(check_nb_line("ERORLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("_print", cout_content, 1));
}
TEST_F(tee_logger, warning_lvl)
{
    //set log level
    io::LoggerManager::SetLogLevel(io::LogLevel::WARNING);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    EXTDLOG << "EXTDLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = get_cout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 2));
    //Test console
    EXPECT_TRUE(check_nb_line("ERORLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("_print", cout_content, 2));
}
TEST_F(tee_logger, info_lvl)
{
    //set log level
    io::LoggerManager::SetLogLevel(io::LogLevel::INFO);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    EXTDLOG << "EXTDLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = get_cout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 3));
    //Test console
    EXPECT_TRUE(check_nb_line("ERORLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("_print", cout_content, 3));
}
TEST_F(tee_logger, extended_lvl)
{
    //set log level
    io::LoggerManager::SetLogLevel(io::LogLevel::EXTENDED);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    EXTDLOG << "EXTDLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content =  get_cout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 4));
    //Test console
    EXPECT_TRUE(check_nb_line("ERORLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", cout_content, 0));
    EXPECT_TRUE(check_nb_line("_print", cout_content, 4));
}
TEST_F(tee_logger, debug_lvl)
{
    //set log level
    io::LoggerManager::SetLogLevel(io::LogLevel::DEBUG);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    EXTDLOG << "EXTDLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content =  get_cout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("_print", log_content, 5));
    //Test console
    EXPECT_TRUE(check_nb_line("ERORLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("EXTDLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", cout_content, 1));
    EXPECT_TRUE(check_nb_line("_print", cout_content, 5));
}

/**
 * LogSession: loggers of a thread for the lifetime of the session
 */
namespace
{
    std::string read_file(std::filesystem::path const &path)
    {
        std::ifstream file(path);
        std::ostringstream content;
        content << file.rdbuf();
        return content.str();
    }

    /** temporary directory, removed at destruction */
    struct TempDir
    {
        std::filesystem::path path;
        explicit TempDir(std::string const &name)
            : path(std::filesystem::temp_directory_path() / name)
        {
            std::filesystem::remove_all(path);
        }
        ~TempDir()
        {
            std::filesystem::remove_all(path);
        }
    };
} // namespace

TEST(log_session, logs_in_its_directory_and_closes_the_files)
{
    TempDir dir("nael_log_session_dir");
    {
        io::LogSession session(dir.path / "sub", io::LogLevel::INFO, "main", false);
        INFOLOG << "INFOLOG_print" << std::endl;
        DBUGLOG << "DBUGLOG_print" << std::endl;
        INFO(io::LoggerManager::GetFileLogger("strategy")) << "strategy_print" << std::endl;
    }
    // the files are closed (flushed) at the end of the session
    std::string main_content = read_file(dir.path / "sub" / "main.log");
    EXPECT_TRUE(check_nb_line("INFOLOG_print", main_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", main_content, 0));
    EXPECT_TRUE(check_nb_line("strategy_print", read_file(dir.path / "sub" / "strategy.log"), 1));
}

TEST(log_session, console_can_be_disabled)
{
    TempDir dir("nael_log_session_console");
    std::stringstream cout;
    std::streambuf *backup = std::cout.rdbuf(cout.rdbuf());
    {
        io::LogSession session(dir.path, io::LogLevel::INFO, "quiet", false);
        INFOLOG << "quiet_print" << std::endl;
    }
    {
        io::LogSession session(dir.path, io::LogLevel::INFO, "loud", true);
        INFOLOG << "loud_print" << std::endl;
    }
    std::cout.rdbuf(backup);
    EXPECT_TRUE(check_nb_line("quiet_print", cout.str(), 0));
    EXPECT_TRUE(check_nb_line("loud_print", cout.str(), 1));
    EXPECT_TRUE(check_nb_line("quiet_print", read_file(dir.path / "quiet.log"), 1));
}

TEST(log_session, does_not_change_the_global_state)
{
    TempDir dir("nael_log_session_global");
    io::LoggerManager::SetDefaultName(LOG_NAME);
    io::LogLevel global_level = io::LoggerManager::GetFileLogger("probe_before").getLogLevel();
    {
        io::LogSession session(dir.path, io::LogLevel::DEBUG, "session", false);
        EXPECT_EQ(io::LoggerManager::GetLogger().getLogLevel(), io::LogLevel::DEBUG);
        // changes the default of the session only
        io::LoggerManager::SetDefaultLogLevel(io::LogLevel::ERROR);
        io::LoggerManager::SetDefaultName("renamed");
        {
            // nested session
            io::LogSession nested(dir.path / "nested", io::LogLevel::WARNING, "nested", false);
            EXPECT_EQ(io::LoggerManager::GetLogger().getLogLevel(), io::LogLevel::WARNING);
        }
        // back to the outer session
        EXPECT_EQ(io::LoggerManager::GetLogger().getLogLevel(), io::LogLevel::ERROR);
        EXPECT_EQ(io::LoggerManager::GetFileLogger("strategy").getLogLevel(), io::LogLevel::ERROR);
    }
    EXPECT_EQ(io::LoggerManager::GetFileLogger("probe_after").getLogLevel(), global_level);
    io::LoggerManager::ClearFileLogger("probe_before");
    io::LoggerManager::ClearFileLogger("probe_after");
    std::filesystem::remove("probe_before.log");
    std::filesystem::remove("probe_after.log");
    EXPECT_TRUE(std::filesystem::exists(dir.path / "nested" / "nested.log"));
    EXPECT_TRUE(std::filesystem::exists(dir.path / "renamed.log"));
    EXPECT_FALSE(std::filesystem::exists("renamed.log"));
}

TEST(log_session, threads_are_independent)
{
    TempDir dir("nael_log_session_threads");
    constexpr int nb_threads = 8;
    std::vector<std::thread> threads;
    for (int t = 0; t < nb_threads; ++t)
    {
        threads.emplace_back([&dir, t]()
        {
            io::LogSession session(dir.path / std::to_string(t), io::LogLevel::INFO, "main", false);
            for (int i = 0; i < 1000; ++i)
            {
                INFOLOG << "thread_" << t << "_print" << std::endl;
                INFO(io::LoggerManager::GetFileLogger("strategy_" + std::to_string(i % 10))) << "x" << std::endl;
            }
        });
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    for (int t = 0; t < nb_threads; ++t)
    {
        std::string content = read_file(dir.path / std::to_string(t) / "main.log");
        EXPECT_TRUE(check_nb_line("thread_" + std::to_string(t) + "_print", content, 1000));
        EXPECT_TRUE(check_nb_line("_print", content, 1000));
    }
}

TEST(log_session, off_level_creates_no_file)
{
    TempDir dir("nael_log_session_off");
    {
        io::LogSession session(dir.path, io::LogLevel::OFF, "main", true);
        INFOLOG << "INFOLOG_print" << std::endl;
        EROR(io::LoggerManager::GetFileLogger("strategy")) << "strategy_print" << std::endl;
        // raising the level afterwards opens the file on the first write
        io::LoggerManager::SetFileLogLevel(io::LogLevel::INFO, "raised");
        INFO(io::LoggerManager::GetFileLogger("raised")) << "raised_print" << std::endl;
    }
    EXPECT_FALSE(std::filesystem::exists(dir.path / "main.log"));
    EXPECT_FALSE(std::filesystem::exists(dir.path / "strategy.log"));
    EXPECT_TRUE(check_nb_line("raised_print", read_file(dir.path / "raised.log"), 1));
}

TEST(log_session, off_level_removes_the_stale_file)
{
    TempDir dir("nael_log_session_stale");
    {
        io::LogSession session(dir.path, io::LogLevel::INFO, "main", false);
        INFOLOG << "previous_run_print" << std::endl;
    }
    ASSERT_TRUE(std::filesystem::exists(dir.path / "main.log"));
    {
        io::LogSession session(dir.path, io::LogLevel::OFF, "main", false);
        INFOLOG << "INFOLOG_print" << std::endl;
    }
    EXPECT_FALSE(std::filesystem::exists(dir.path / "main.log"));
}

TEST(log_session, pool_jobs_use_the_session_of_the_enqueuing_thread)
{
    TempDir dir("nael_log_session_pool");
    auto run = [&dir](int r)
    {
        io::LogSession session(dir.path / std::to_string(r), io::LogLevel::INFO, "main", false);
        thread::Pool pool(4);
        for (int s = 0; s < 8; ++s)
        {
            pool.enqueue([s]()
            {
                std::string name = "strategy_" + std::to_string(s);
                for (int i = 0; i < 200; ++i)
                {
                    INFO(io::LoggerManager::GetFileLogger(name)) << name << "_print" << std::endl;
                }
            });
        }
        pool.joinAll();
    };
    std::thread run0(run, 0);
    std::thread run1(run, 1);
    run0.join();
    run1.join();
    for (int r = 0; r < 2; ++r)
    {
        for (int s = 0; s < 8; ++s)
        {
            std::string name = "strategy_" + std::to_string(s);
            EXPECT_TRUE(check_nb_line(name + "_print", read_file(dir.path / std::to_string(r) / (name + ".log")), 200));
            EXPECT_FALSE(std::filesystem::exists(name + ".log"));
        }
    }
}
