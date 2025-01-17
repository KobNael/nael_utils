#include <gtest/gtest.h>

#include <nael_utils/log/log.hh>

#define LOG_NAME "test_logger.log"

//Tools for tests
namespace{
    //Parse the log file
    std::string get_log_content()
    {
        std::ostringstream log_content;
        // Parse line by line log file
        std::ifstream log(LOG_NAME);
        if (!log.is_open())
        {
            throw std::runtime_error( "Failed to open log file" );
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
    void check_nb_line(std::string pattern, std::string content, unsigned ref)
    {
        unsigned nb = 0;
        std::string::size_type pos = 0;
        while ((pos = content.find(pattern, pos )) != std::string::npos) {
                ++ nb;
                pos += pattern.length();
        }
        ASSERT_EQ(nb, ref);
   }
} //namespace

/**
 * Singleton get or create
 */
TEST(tee_logger_singleton, singleton_access)
{
    //No log by default
    ASSERT_THROW(io::GetLogger(), io::access_error);
    //Create it
    io::CreateLogger(LOG_NAME);
    ASSERT_NO_THROW(io::GetLogger());
    //Free it
    io::ClearLogger();
    ASSERT_THROW(io::GetLogger(), io::access_error);
}


/**
 * FileLogger usage
 */
class file_logger: public ::testing::Test {
protected:
    //SetUp (init logger)
    virtual void SetUp()
    {
        _file_logger = new io::FileLogger(LOG_NAME);
    }

    //TearDown (clear logger)
    virtual void TearDown()
    {
        delete _file_logger;
        _file_logger = nullptr;
    }

    //Access to logger
    io::FileLogger *get_log()
    {
        return _file_logger;
    }

private:
    io::FileLogger *_file_logger = {nullptr};
};

TEST_F(file_logger, off_level)
{
    //set log level
    get_log()->setLogLevel(io::OFF_LVL);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 0);
    check_nb_line("WARNLOG_print", log_content, 0);
    check_nb_line("INFOLOG_print", log_content, 0);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 0);
}

TEST_F(file_logger, error_lvl)
{
    //set log level
    get_log()->setLogLevel(io::ERROR_LVL);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 0);
    check_nb_line("INFOLOG_print", log_content, 0);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 1);
}
TEST_F(file_logger, warning_lvl)
{
    //set log level
    get_log()->setLogLevel(io::WARNING_LVL);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 1);
    check_nb_line("INFOLOG_print", log_content, 0);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 2);
}
TEST_F(file_logger, info_lvl)
{
    //set log level
    get_log()->setLogLevel(io::INFO_LVL);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 1);
    check_nb_line("INFOLOG_print", log_content, 1);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 3);
}
TEST_F(file_logger, debug_lvl)
{
    //set log level
    get_log()->setLogLevel(io::DEBUG_LVL);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 1);
    check_nb_line("INFOLOG_print", log_content, 1);
    check_nb_line("DBUGLOG_print", log_content, 1);
    check_nb_line("_print", log_content, 4);
}
TEST_F(file_logger, default_level)
{
    //do not set log level
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 1);
    check_nb_line("INFOLOG_print", log_content, 1);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 3);
}

/**
 * TeeLogger usage
 */
class tee_logger: public ::testing::Test {
protected:
    //SetUp (init logger)
    virtual void SetUp()
    {
        io::CreateLogger(LOG_NAME);
    }

    //TearDown (clear logger)
    virtual void TearDown()
    {
        io::ClearLogger();
    }
};

TEST_F(tee_logger, off_level)
{
    //Start capturing cout
    testing::internal::CaptureStdout();
    //set log level
    io::SetLogLevel(io::OFF_LVL);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = testing::internal::GetCapturedStdout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 0);
    check_nb_line("WARNLOG_print", log_content, 0);
    check_nb_line("INFOLOG_print", log_content, 0);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 0);
    //Test console
    check_nb_line("ERORLOG_print", cout_content, 0);
    check_nb_line("WARNLOG_print", cout_content, 0);
    check_nb_line("INFOLOG_print", cout_content, 0);
    check_nb_line("DBUGLOG_print", cout_content, 0);
    check_nb_line("_print", log_content, 0);
}

TEST_F(tee_logger, error_lvl)
{
    //Start capturing cout
    testing::internal::CaptureStdout();
    //set log level
    io::SetLogLevel(io::ERROR_LVL);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = testing::internal::GetCapturedStdout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 0);
    check_nb_line("INFOLOG_print", log_content, 0);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 1);
    //Test console
    check_nb_line("ERORLOG_print", cout_content, 1);
    check_nb_line("WARNLOG_print", cout_content, 0);
    check_nb_line("INFOLOG_print", cout_content, 0);
    check_nb_line("DBUGLOG_print", cout_content, 0);
    check_nb_line("_print", log_content, 1);
}
TEST_F(tee_logger, warning_lvl)
{
    //Start capturing cout
    testing::internal::CaptureStdout();
    //set log level
    io::SetLogLevel(io::WARNING_LVL);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = testing::internal::GetCapturedStdout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 1);
    check_nb_line("INFOLOG_print", log_content, 0);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 2);
    //Test console
    check_nb_line("ERORLOG_print", cout_content, 1);
    check_nb_line("WARNLOG_print", cout_content, 1);
    check_nb_line("INFOLOG_print", cout_content, 0);
    check_nb_line("DBUGLOG_print", cout_content, 0);
    check_nb_line("_print", log_content, 2);
}
TEST_F(tee_logger, info_lvl)
{
    //Start capturing cout
    testing::internal::CaptureStdout();
    //set log level
    io::SetLogLevel(io::INFO_LVL);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = testing::internal::GetCapturedStdout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 1);
    check_nb_line("INFOLOG_print", log_content, 1);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 3);
    //Test console
    check_nb_line("ERORLOG_print", cout_content, 1);
    check_nb_line("WARNLOG_print", cout_content, 1);
    check_nb_line("INFOLOG_print", cout_content, 1);
    check_nb_line("DBUGLOG_print", cout_content, 0);
    check_nb_line("_print", log_content, 3);
}
TEST_F(tee_logger, debug_lvl)
{
    //Start capturing cout
    testing::internal::CaptureStdout();
    //set log level
    io::SetLogLevel(io::DEBUG_LVL);
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = testing::internal::GetCapturedStdout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 1);
    check_nb_line("INFOLOG_print", log_content, 1);
    check_nb_line("DBUGLOG_print", log_content, 1);
    check_nb_line("_print", log_content, 4);
    //Test console
    check_nb_line("ERORLOG_print", cout_content, 1);
    check_nb_line("WARNLOG_print", cout_content, 1);
    check_nb_line("INFOLOG_print", cout_content, 1);
    check_nb_line("DBUGLOG_print", cout_content, 1);
    check_nb_line("_print", log_content, 4);
}
TEST_F(tee_logger, default_level)
{
    //Start capturing cout
    testing::internal::CaptureStdout();
    //do not set log level
    //Send the content
    ERORLOG << "ERORLOG_print" << std::endl;
    WARNLOG << "WARNLOG_print" << std::endl;
    INFOLOG << "INFOLOG_print" << std::endl;
    DBUGLOG << "DBUGLOG_print" << std::endl;
    //stops capturing stdout and returns the captured string
    std::string cout_content = testing::internal::GetCapturedStdout();
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    check_nb_line("ERORLOG_print", log_content, 1);
    check_nb_line("WARNLOG_print", log_content, 1);
    check_nb_line("INFOLOG_print", log_content, 1);
    check_nb_line("DBUGLOG_print", log_content, 0);
    check_nb_line("_print", log_content, 3);
    //Test console
    check_nb_line("ERORLOG_print", cout_content, 1);
    check_nb_line("WARNLOG_print", cout_content, 1);
    check_nb_line("INFOLOG_print", cout_content, 1);
    check_nb_line("DBUGLOG_print", cout_content, 0);
    check_nb_line("_print", log_content, 3);
}
