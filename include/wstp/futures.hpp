#pragma once

#include <mutex>
#include <condition_variable>
#include <exception>
#include <memory>
#include <optional>

namespace wstp {
    template<typename T>
    class FutureState {
    public:
        void set_value(T val) {
            std::lock_guard<std::mutex> lock(mtx_);
            if (ready_) throw std::runtime_error("Value already set");
            value_ = std::move(val);
            ready_ = true;
        
        cv_.notify_all();
        }
    
    private:
        mutable std::mutex mtx_;
        std::condition_variable cv_;
        std::optional<T> value_;
        std::exception_ptr exception_;
        bool ready_ = false;
    };
}