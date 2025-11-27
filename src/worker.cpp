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
        //random engine for stealing
        std::mt19937 rng(std::random_device{}());
        size_t num_workers = pool_->size();

        while(!stop_.load(std::memory_order_relaxed)) {
            std::optional<Task> task;

            task = localQueue_->pop_bottom();

            if(!task) {
                for(size_t i = 0; i < num_workers;i++) {
                    if( i == id_) continue;

                    auto& other_queue = pool_->queues_[i];
                    task = other_queue->steal_top();
                    if(task) break;
                }
            }
        }
    }
}