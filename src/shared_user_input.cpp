#include "shared_user_input.hpp"

#include "move.hpp"
#include "shared_move.hpp"

namespace chess {

Shared_move Shared_user_input::move;

std::atomic<bool> Shared_user_input::exit_requested{false};

}  // namespace chess
