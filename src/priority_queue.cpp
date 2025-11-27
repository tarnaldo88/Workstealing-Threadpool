#include "priority_queue.hpp"

void PrioritizedTaskQueue::push(wstp::Task task, TaskPriority priority)
{
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
