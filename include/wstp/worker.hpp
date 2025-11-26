#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include "task.hpp"
#include "work_stealing_queue.hpp"

namespace wstp {
    class Worker {
        public:
        Worker(size_t id, ThreadPool* pool, WorkStealingQueue* localQueue);
        void notify_stop();
        void join();
    };
 }
