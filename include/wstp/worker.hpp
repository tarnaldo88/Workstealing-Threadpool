#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include "task.hpp"
#include "work_stealing_queue.hpp"

namespace wstp {

    class ThreadPool;

    class Worker {
    public:
        Worker(size_t id, ThreadPool* pool, WorkStealingQueue* localQueue);
        ~Worker();

        Worker(const Worker&) = delete;
        Worker& operator=(const Worker&) = delete;

        Worker(Worker&&) = delete;
        Worker& operator=(Worker&&) = delete;

        void notify_stop();
        void join();

    private:
        void run();

        size_t id_;
        ThreadPool* pool_;
        WorkStealingQueue* localQueue_;
        std::thread thread_;
        std::atomic<bool> stop_{false};
    };
 }
