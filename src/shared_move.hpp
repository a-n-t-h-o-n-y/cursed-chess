#ifndef CHESS_SHARED_MOVE_HPP
#define CHESS_SHARED_MOVE_HPP
#include <condition_variable>
#include <mutex>

#include "move.hpp"

namespace chess {

/// Thrown when the chess loop has been requested to exit.
struct Chess_loop_exit_request {};

/// Provide an interface to working with a variable accessible across threads.
/** Handles spurious wakeups internally. */
class Shared_move {
   public:
    /// Set the state of the variable and notify all threads calling get().
    void set(const Move& value);

    /// Blocking call to retrieve the shared state value.
    /** Can only be called once per notification from another thread. */
    Move get(std::unique_lock<std::mutex>& ul) const;

    /// Provided so caller can control exclusive access to shared state.
    mutable std::mutex mtx;

   private:
    Move variable_;
    mutable std::condition_variable condition_var_;
    mutable bool verify_{false};
};

}  // namespace chess
#endif  // CHESS_SHARED_MOVE_HPP
