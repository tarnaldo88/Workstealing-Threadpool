#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "task.hpp"

enum class TaskPriority : int {
    LOW = 0,
    NORMAL = 1,
    HIGH = 2,
    CRITICAL = 3
};

struct PrioritizedTask {
    wstp::Task task;
    TaskPriority priority;

    bool operator<(const PrioritizedTask& other) const {
        return static_cast<int>(priority) <static_cast<int>(other.priority);
    }
};

class PrioritizedTaskQueue {
public:
    PrioritizedTaskQueue() = default;

    void push(wstp::Task task, TaskPriority priority);

    wstp::Task pop();

    bool try_pop(wstp::Task& out);

    bool empty() const;

private:
    mutable std::mutex mtx_;

    std::condition_variable cv_;
    std::priority_queue<PrioritizedTask> queue_;
};