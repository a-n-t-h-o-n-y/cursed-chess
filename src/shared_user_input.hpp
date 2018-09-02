#ifndef CHESS_SHARED_USER_INPUT_HPP
#define CHESS_SHARED_USER_INPUT_HPP
#include "move.hpp"
#include "shared_variable.hpp"

namespace chess {

struct Shared_user_input {
    static Shared_variable<Move> move;
};

}  // namespace chess
#endif  // CHESS_SHARED_USER_INPUT_HPP
