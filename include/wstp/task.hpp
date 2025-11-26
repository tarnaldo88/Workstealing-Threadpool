#pragma once

#include <functional>
#include <utility>

namespace wstp {
    class Task {
    public:
        Task() = default;

        template <typename F>
        Task(F&& f) : func_(std::forward<F>(f)) {}

        //move only
        Task(Task&& other) noexcept = default;
        Task& operator=(Task&& other) noexcept = default;

        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;
        // Execute the task
        void operator()() {
            if (func_) {
                func_();
            }
        }

        explicit operator bool() const noexcept {
            return static_cast<bool>(func_);
        }

    private:
        std::function<void()> func_;
    };
}