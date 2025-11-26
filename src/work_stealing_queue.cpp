#include "wstp/work_stealing_queue.hpp"

#include <thread>

namespace wstp {
    void WorkStealingQueue::push_bottom(Task&& task) {
        std::lock_guard<std::mutex> lock(mutex_);
        deque_.push_back(std::move(task));
    }

    std::optional<Task> WorkStealingQueue::pop_bottom() {
        std::lock_guard<std::mutex> lock(mutex_);
        if(deque_.empty()) {
            return std::nullopt;
        }

        Task task = std::move(deque_.back());
        deque_.pop_back();
        return task;
    }

    std::optional<Task> WorkStealingQueue::steal_top() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (deque_.empty())
            return std::nullopt;

        Task task = std::move(deque_.front());
        deque_.pop_front();
        return task;
    }
}