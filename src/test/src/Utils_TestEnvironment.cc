#include <gtest/gtest.h>

#include <nael_utils/log/log.hh>

class Utils_TestEnvironment : public ::testing::Environment
{
    public:
        ~Utils_TestEnvironment() override {}

        // Override this to define how to set up the environment.
        void SetUp() override {
            io::LoggerManager::SetDefaultLogLevel( io::LogLevel::OFF );
        }

        // Override this to define how to tear down the environment.
        void TearDown() override {
            io::LoggerManager::ClearAllLoggers();
        }
};

::testing::Environment* const ps_env = ::testing::AddGlobalTestEnvironment(new Utils_TestEnvironment);
