#include "shared_move.hpp"

#include <chrono>
#include <mutex>

#include "move.hpp"
#include "shared_user_input.hpp"

namespace chess {

/// Set the state of the variable and notify any threads calling get().
void Shared_move::set(const Move& value) {
    {
        std::lock_guard<std::mutex> lock{mtx};
        variable_ = value;
        verify_ = true;
    }
    condition_var_.notify_all();
}

Move Shared_move::get(std::unique_lock<std::mutex>& ul) const {
    do {
        condition_var_.wait_for(ul, std::chrono::milliseconds(250));
        if (Shared_user_input::exit_requested) {
            throw Chess_loop_exit_request{};
        }
    } while (!verify_);
    verify_ = false;
    return variable_;
}
}  // namespace chess
