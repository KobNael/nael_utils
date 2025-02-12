#include <gtest/gtest.h>

#include <nael_utils/log/log.hh>

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
    io::FileLogger &get_log()
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
    get_log().setLogLevel(io::LogLevel::OFF);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
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
    get_log().setLogLevel(io::LogLevel::ERROR);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
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
    get_log().setLogLevel(io::LogLevel::WARNING);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
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
    get_log().setLogLevel(io::LogLevel::INFO);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
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
    get_log().setLogLevel(io::LogLevel::DEBUG);
    //Send the content
    EROR(get_log()) << "ERORLOG_print" << std::endl;
    WARN(get_log()) << "WARNLOG_print" << std::endl;
    INFO(get_log()) << "INFOLOG_print" << std::endl;
    DBUG(get_log()) << "DBUGLOG_print" << std::endl;
    //get log file content
    std::string log_content = get_log_content();
    //Test file
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("_print", log_content, 4));
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
    EXPECT_TRUE(check_nb_line("ERORLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("WARNLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("INFOLOG_print", log_content, 1));
    EXPECT_TRUE(check_nb_line("DBUGLOG_print", log_content, 0));
    EXPECT_TRUE(check_nb_line("_print", log_content, 3));
}

/**
 * TeeLogger usage
 */
class tee_logger: public ::testing::Test {
protected:
    void SetUp() override
    {
        io::LoggerManager::SetDefaultName(LOG_NAME);
        _cout.str("");
        _cout.clear();
        std::cout.rdbuf(_cout.rdbuf());
    }
    void TearDown() override
    {
        io::LoggerManager::ClearLogger();
    }
    std::string get_cout()
    {
        return _cout.str();
    }
private:
    std::stringstream _cout;
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
TEST_F(tee_logger, default_level)
{
    //do not set log level
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
