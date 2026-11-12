#include <gtest/gtest.h>

#include <nael_utils/thread/pool.hh>
#include <chrono>
#include <thread>

namespace
{
    void sumInt(int &val, std::mutex &mtx)
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ++val;
    }
}

TEST(thread_pool, pool_size)
{
    thread::Pool pool(4);
    ASSERT_EQ(pool.get_pool_size(), 4);
    pool.joinAll();
    thread::Pool pool_default;
    ASSERT_EQ(pool_default.get_pool_size(), std::thread::hardware_concurrency());
    pool_default.joinAll();
    thread::Pool pool_zero(0);
    ASSERT_EQ(pool_zero.get_pool_size(), std::thread::hardware_concurrency());
    pool_zero.joinAll();
}

TEST(thread_pool, simpleTest)
{
    thread::Pool pool(4);
    ASSERT_EQ(pool.get_pool_size(), 4);

    int val(0);
    int target(20);
    std::mutex mtx;
    for(int i(0); i<target; ++i)
    {
        pool.enqueue(std::bind(sumInt, std::ref(val), std::ref(mtx)));
    }

    pool.joinAll();

    EXPECT_EQ(target, val);
}

