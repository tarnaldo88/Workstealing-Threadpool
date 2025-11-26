#pragma once

#include <deque>
#include <mutex>
#include <optional>
#include "task.hpp"

namespace wstp {
    class WorkStealingQueue {
        public:
            WorkStealingQueue() = default;
            ~WorkStealingQueue() = default;

            WorkStealingQueue(const WorkStealingQueue&) = delete;
            WorkStealingQueue& operator=(const WorkStealingQueue&) = delete;

            void push_bottom(Task&& task);
            std::optional<Task> pop_bottom();
            std::optional<Task> steal_top();

        private:
            std::deque<Task> deque_;
            std::mutex mutex_;
    };
}