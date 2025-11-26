#include "wstp/thread_pool.hpp"
#include "wstp/worker.hpp"
#include "wstp/work_stealing_queue.hpp"
#include "wstp/task.hpp"

#include <thread>

namespace wstp {
    ThreadPool::ThreadPool(size_t thread_count) {
        if(thread_count == 0) {
            thread_count = std::max<size_t>(1, std::thread::hardware_concurrency());
        }

        queues_.reserve(thread_count);
        workers_.reserve(thread_count);

        //each worker gets one work-stealing queue
        for(size_t i = 0; i < thread_count; i++) {
            queues_.emplace_back(std::make_unique<WorkStealingQueue>());
        }

        start_workers(thread_count);
    }
}