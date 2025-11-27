#include "priority_queue.hpp"

void PrioritizedTaskQueue::push(wstp::Task task, TaskPriority priority)
{
    std::lock_guard<std::mutex> lock(mtx_);
    queue_.push(PrioritizedTask{task, priority});
    cv_.notify_one();
}

wstp::Task PrioritizedTaskQueue::pop()
{
    std::unique_lock<std::mutex> lock(mtx_);

    cv_.wait(lock, [this]{
        return !queue_.empty();
    });

    auto pt = queue_.top();
    queue_.pop();    
    return pt.task;
}

bool PrioritizedTaskQueue::try_pop(wstp::Task &out)
{
    return false;
}

bool PrioritizedTaskQueue::empty() const
{
    return queue_.empty();
}
