#ifndef CHESS_SHARED_VARIABLE_HPP
#define CHESS_SHARED_VARIABLE_HPP
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <utility>

#include <cppurses/system/system.hpp>

namespace chess {

struct System_exit_request {};

/// Provides a simplified interface to working with a varaible accessible across
/// threads. Handles spurious wakeups internally.
template <typename T>
class Shared_variable {
   public:
    /// Forwards args to the T constructor. Useful if no default constructor.
    /// This initial state will not be accessible from get().
    template <typename... Args>
    Shared_variable(Args&&... args);

    /// Sets the state of the variable and notifies all threads calling get().
    void set(const T& value);

    /// Sets the state of the variable and notifies all threads calling get().
    void set(T&& value);

    /// Blocking call that retrieves the shared state once notified of a change.
    /// Can only be called onces per notification from another thread.
    T& get(std::unique_lock<std::mutex>& ul);

    /// Retrieves the shared state value.
    /// Can only be called onces per notification from another thread.
    const T& get(std::unique_lock<std::mutex>& ul) const;

    /// Provided so caller can control exclusive access to shared state.
    mutable std::mutex mtx;

   private:
    T variable_;
    mutable std::condition_variable condition_var_;
    mutable bool verify_{false};
};

template <typename T>
template <typename... Args>
Shared_variable<T>::Shared_variable(Args&&... args)
    : variable_{std::forward<Args>(args)...} {}

/// Sets the state of the variable and notifies any threads calling get().
template <typename T>
void Shared_variable<T>::set(const T& value) {
    {
        std::lock_guard<std::mutex> lock{mtx};
        variable_ = value;
        verify_ = true;
    }
    condition_var_.notify_all();
}

template <typename T>
void Shared_variable<T>::set(T&& value) {
    {
        std::lock_guard<std::mutex> lock{mtx};
        variable_ = value;
        verify_ = true;
    }
    condition_var_.notify_all();
}

template <typename T>
T& Shared_variable<T>::get(std::unique_lock<std::mutex>& ul) {
    do {
        condition_var_.wait_for(ul, std::chrono::milliseconds(250));
        // TODO would be better to exit on Chess_event_loop::exit()
        if (cppurses::System::exit_requested()) {
            throw System_exit_request{};
        }
    } while (!verify_);
    verify_ = false;
    return variable_;
}

template <typename T>
const T& Shared_variable<T>::get(std::unique_lock<std::mutex>& ul) const {
    do {
        condition_var_.wait(ul);
    } while (!verify_);
    verify_ = false;
    return variable_;
}

}  // namespace chess
#endif  // CHESS_SHARED_VARIABLE_HPP
