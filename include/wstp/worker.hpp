// #include "thread_pool.hpp"

namespace wstp {
    class Worker {
        public:
        Worker(size_t id, ThreadPool* pool, WorkStealingQueue* localQueue);
        void notify_stop();
        void join();
    };
 }
