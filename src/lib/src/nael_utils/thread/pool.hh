/**
 * @file pool.hh
 */
#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

namespace thread
{
    /**
     * @class Pool
     * @brief Utility for parallel computing
     *
     * This class allow the user to enqueue a list of jobs to process
     * and then to process them using as many subthreads as wanted.
     *
     * @code{cpp}
     * //Increase the value of an int
     * void sumInt(int &val, std::mutex &mtx)
     * {
     *      std::unique_lock<std::mutex> lock(mtx);
     *      std::this_thread::sleep_for(std::chrono::milliseconds(100));
     *      ++val;
     * }
     *
     *  //Create a pool, with 4 workers
     *  thread::Pool pool(4);
     *  int val(0);
     *  int target(20);
     *  std::mutex mtx;
     *  //Enqueu 20 calls to sumInt
     *  for(int i(0); i<target; ++i)
     *  {
     *      pool.enqueue(std::bind(sumInt, std::ref(val), std::ref(mtx)));
     *  }
     *  //Launch the execution and wait for termination
     *  pool.joinAll();
     * @endcode
     */
    class Pool
    {
    private:
        /** @brief definition of a job */
        typedef std::function<void()> job_t;

        /** @brief queue of jobs to be done */
        std::deque<job_t> _queue;

        /** @brief list of workers */
        std::vector<std::thread> _workers;

        /** @brief mutex for thread synchronization */
        std::mutex _mutex;

        /** @brief condition variable for signaling */
        std::condition_variable _cond;

        /** @brief signal for termination */
        std::atomic<bool> _shutdown = {false};

    public:
        /**
         * @brief Add a job in the queue
         * @param job the job
         */
        void enqueue(job_t job)
        {
            // Acquire lock
            std::unique_lock<std::mutex> lock(_mutex);
            // Add item
            _queue.push_back(job);
            // Notify one thread that is waiting
            _cond.notify_one();
        }

        /**
         * @brief Thread safe accessor to the next job
         * @return the next job if any, {} otherwise
         */
        std::optional<job_t> dequeue()
        {
            // acquire lock
            std::unique_lock<std::mutex> lock(_mutex);
            // wait until queue is not empty
            _cond.wait(lock,
                       [this]()
                       { return _shutdown || !_queue.empty(); });
            if (_queue.empty())
            {
                return {};
            }

            job_t job = _queue.front();
            _queue.pop_front();
            return job;
        }

        /**
         * @brief Launch all the threads and wait for termination
         */
        void joinAll()
        {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _shutdown = true;
                _cond.notify_all();
            }
            for (std::thread &t : _workers)
            {
                t.join();
            }
        }

        size_t get_pool_size() const
        {
            return _workers.size();
        }

        size_t get_queue_size() const
        {
            return _queue.size();
        }

    private:
        /** @brief Worker implementation */
        static void worker(Pool &p)
        {
            while (std::optional<job_t> j = p.dequeue())
            {
                try
                {
                    (*j)();
                }
                catch (const std::exception &e)
                {
                    //@todo
                }
            }
        }

    public:
        /**
         * @brief Constructor
         * @param nworkers the number of threads
         */
        explicit Pool(size_t nworkers = 0)
        {
            if (0 == nworkers)
            {
                nworkers = std::thread::hardware_concurrency();
            }
            for (size_t i(0); i < nworkers; ++i)
            {
                _workers.push_back(
                    std::thread(std::bind(worker, std::ref(*this))));
            }
        }
    };
} // namespace thread
