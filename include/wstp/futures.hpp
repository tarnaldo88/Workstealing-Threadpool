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
            {
                std::lock_guard<std::mutex> lock(mtx_);
                if (ready_) throw std::runtime_error("Value already set");
                value_ = std::move(val);
                ready_ = true;
            }        
            cv_.notify_all();
        }

        void set_exception(std::exception_ptr ex) {
            {
                std::lock_guard<std::mutex> lock(mtx_);
                if (ready_) throw std::runtime_error("Value already set");
                exception_ = ex;
                ready_ = true;
            }
            cv_.notify_all();
        }

         T get() {
            std::unique_lock<std::mutex> lock(mtx_);

            cv_.wait(lock, [this]{
                return ready_;
            });

            if (exception_) std::rethrow_exception(exception_);

            return std::move(*value_);
        }

        bool is_ready() const {
            std::lock_guard<std::mutex> lock(mtx_);
            return ready_;
        }
    
    private:
        mutable std::mutex mtx_;
        std::condition_variable cv_;
        std::optional<T> value_;
        std::exception_ptr exception_;
        bool ready_ = false;
    };
    template<typename T>
    class Future {
    public:
        Future() = default;
        explicit Future(std::shared_ptr<FutureState<T>> st) : state_(st) {}

        T get() {
            if (!state_) throw std::runtime_error("Invalid future");
            return state_->get();
        }

        bool valid() const { return (bool)state_; }

    private:
        std::shared_ptr<FutureState<T>> state_;
    };

    
    // Promise<T>    
    template<typename T>
    class Promise {
    public:
        Promise() : state_(std::make_shared<FutureState<T>>()) {}

        Future<T> get_future() {
            return Future<T>(state_);
        }

        void set_value(T val) {
            state_->set_value(std::move(val));
        }

        void set_exception(std::exception_ptr ex) {
            state_->set_exception(ex);
        }

    private:
        std::shared_ptr<FutureState<T>> state_;
    };

    
    // Convenience: Promise<void>/Future<void>
    template<>
    class Future<void> {
    public:
        Future() = default;
        explicit Future(std::shared_ptr<FutureState<int>> st) : dummy_state_(st) {}

        void get() {
            if (!dummy_state_) throw std::runtime_error("Invalid void future");
            dummy_state_->get(); // returns int but unused
        }

    private:
        std::shared_ptr<FutureState<int>> dummy_state_; // dummy type
    };

    template<>
    class Promise<void> {
    public:
        Promise() : dummy_state_(std::make_shared<FutureState<int>>()) {}

        Future<void> get_future() {
            return Future<void>(dummy_state_);
        }

        void set_value() {
            dummy_state_->set_value(0);
        }

        void set_exception(std::exception_ptr ex) {
            dummy_state_->set_exception(ex);
        }

    private:
        std::shared_ptr<FutureState<int>> dummy_state_;
    };
}