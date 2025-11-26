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
}