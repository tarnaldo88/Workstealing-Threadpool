# Work-Stealing Thread Pool (WSTP)

A high-performance C++ thread pool implementation using work-stealing queues for efficient task distribution and load balancing.

## Overview

WSTP is a modern C++ library that provides a thread pool with work-stealing capabilities. It's designed to efficiently distribute tasks across multiple worker threads, minimizing contention and maximizing CPU utilization through intelligent work distribution.

### Key Features

- **Work-Stealing Architecture**: Each worker thread has its own queue, and idle workers can steal tasks from other workers' queues
- **Priority-Based Scheduling**: Submit tasks with priority levels for urgent work
- **Future-Based API**: Asynchronous task submission with `std::future` support
- **Lock-Free Optimization**: Minimizes synchronization overhead through work-stealing
- **Automatic Thread Count**: Defaults to hardware concurrency if not specified
- **Move-Only Tasks**: Efficient task ownership semantics

## Architecture

### Components

- **ThreadPool**: Main interface for submitting and scheduling tasks
- **Worker**: Individual worker threads that execute tasks from their queues
- **WorkStealingQueue**: Per-worker queue supporting both local and remote task access
- **Task**: Lightweight wrapper for callable objects
- **Future/Promise**: Custom future implementation for task result handling

### How It Works

1. Each worker thread maintains its own task queue
2. Workers primarily pop tasks from the bottom of their own queue (LIFO)
3. Idle workers can steal tasks from the top of other workers' queues (FIFO)
4. This reduces lock contention and improves cache locality
5. Priority scheduling allows urgent tasks to be processed first

## Usage

### Basic Task Submission

```cpp
#include <wstp/thread_pool.hpp>

wstp::ThreadPool pool(4); // Create pool with 4 threads

// Submit a simple task
auto future = pool.submit([]() {
    std::cout << "Task executed\n";
});

// Wait for completion
future.get();
```

### Tasks with Return Values

```cpp
auto future = pool.submit([]() {
    return 42;
});

int result = future.get(); // Returns 42
```

### Priority Scheduling

```cpp
// Schedule with priority (higher = more urgent)
auto urgent = pool.schedule(10, []() {
    std::cout << "High priority task\n";
});

auto normal = pool.schedule(1, []() {
    std::cout << "Normal priority task\n";
});
```

### Tasks with Arguments

```cpp
auto future = pool.submit([](int x, int y) {
    return x + y;
}, 5, 3);

int result = future.get(); // Returns 8
```

## Building

### Requirements

- C++17 or later
- CMake 3.10+
- A modern C++ compiler (GCC, Clang, MSVC)

### Compilation

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running Tests

```bash
cd build
ctest
```

### ThreadPool

```cpp
// Constructor
explicit ThreadPool(size_t thread_count = 0);
// If thread_count is 0, uses std::thread::hardware_concurrency()

// Submit a task and get a future
template<typename F, typename... Args>
auto submit(F&& f, Args&&... args) -> std::future<std::invoke_result<F,Args...>>;

// Submit a prioritized task
template<typename F, typename... Args>
auto schedule(int priority, F&& f, Args&&... args) -> std::future<std::invoke_result<F,Args...>>;

// Get number of worker threads
size_t size() const noexcept;
```

### Future/Promise

```cpp
// Future - represents a future result
template<typename T>
class Future {
    T get();              // Block until result is available
    bool valid() const;   // Check if future is valid
};

// Promise - sets the result
template<typename T>
class Promise {
    Future<T> get_future();
    void set_value(T val);
    void set_exception(std::exception_ptr ex);
};

// Specialization for void
template<>
class Future<void> { /* ... */ };

template<>
class Promise<void> { /* ... */ };
```

## Performance Considerations

- **Work-Stealing**: Reduces lock contention compared to centralized queues
- **Cache Locality**: LIFO access to own queue improves cache hit rates
- **Load Balancing**: Idle workers steal from busy workers, balancing load
- **Priority Scheduling**: Urgent tasks are processed before normal tasks

## Thread Safety

- All public APIs are thread-safe
- Multiple threads can safely submit tasks concurrently
- Futures can be safely shared across threads
- The thread pool is non-copyable and non-movable

## Examples

See the `examples/` directory for complete working examples.

## Testing

The project includes comprehensive tests in the `tests/` directory. Run them with:

```bash
cmake --build build --target test
```