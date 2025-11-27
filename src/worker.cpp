#include "worker.hpp"
#include "thread_pool.hpp"

#include <chrono>
#include <optional>
#include <random>

namespace wstp {
    Worker::Worker(size_t id, ThreadPool *pool, WorkStealingQueue *localQueue) : id_(id), pool_(pool), localQueue_(localQueue) {
        thread_ = std::thread(&Worker::run, this);
    }

    Worker::~Worker() {
        notify_stop();
        join();
    }

    void Worker::notify_stop() {
        stop_.store(true, std::memory_order_relaxed);
    }

    void Worker::join() {
        if(thread_.joinable()) {
            thread_.join();
        }
    }

    //main thread loop
    void Worker::run() {
        
    }
}