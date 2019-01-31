#ifndef CHESS_SHARED_USER_INPUT_HPP
#define CHESS_SHARED_USER_INPUT_HPP
#include <atomic>

#include "move.hpp"
#include "shared_move.hpp"

namespace chess {

struct Shared_user_input {
    static Shared_move move;
    static std::atomic<bool> exit_requested;
};

}  // namespace chess
#endif  // CHESS_SHARED_USER_INPUT_HPP
