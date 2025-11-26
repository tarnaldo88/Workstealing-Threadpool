#pragma once

#include <future>
#include <functional>
#include <vector>
#include <memory>
#include <atomic>

namespace wstp {
    class Worker;
    class WorkStealingQueue;
    struct Task;

    class ThreadPool {
        public:
            explicit ThreadPool(size_t thread_count = 0);

            ~ThreadPool();

            Threadpool(const ThreadPool&) = delete;
            Threadpool& operator=(const ThreadPool&) = delete;

            Threadpool(const ThreadPool&&) = delete;
            Threadpool& operator=(const ThreadPool&&) = delete;

            //submit a task returning a future<T>
            template<typename F, typename... Args>
            auto submit(F&& f, Args&&... args)-> std::future<std::invoke_result<F,Args....>>;

            //Submit prioritzed tasks. Higher = more urgent
            template<typename F, typename... Args>
            auto schedule(int priority,F&& f, Args&&... args)-> std::future<std::invoke_result<F,Args....>>;

            size_t size() const noexcept {return workers_.size();}
        private:
            void start_workers(size_t count);
            void stop_workers();

            std:;atomic<bool> stop_(false);

            //One queue per worker for work stealing
            std::vector<std::unique_ptr<WorkStealingQueue>> queues_;

            //the worker threads
            std::vector<std::unique_ptr<Worker>> workers_;            
    }
}
