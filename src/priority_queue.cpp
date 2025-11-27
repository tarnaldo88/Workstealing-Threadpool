#include "priority_queue.hpp"

void PrioritizedTaskQueue::push(wstp::Task task, TaskPriority priority)
{
    std::lock_guard<std::mutex> lock(mtx_);
    queue_.push(PrioritizedTask{task, priority});
    cv_.notify_one();
}

wstp::Task PrioritizedTaskQueue::pop()
{
    return wstp::Task();
}

bool PrioritizedTaskQueue::try_pop(wstp::Task &out)
{
    return false;
}

bool PrioritizedTaskQueue::empty() const
{
    return false;
}
