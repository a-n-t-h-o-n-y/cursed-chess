#include "player_human.hpp"

#include <mutex>

#include "move.hpp"
#include "shared_user_input.hpp"

class Chess_engine;

namespace chess {

Player_human::Player_human(const Chess_engine& engine) : Player{engine} {}

Move Player_human::get_move() {
    std::unique_lock<std::mutex> ul(Shared_user_input::move.mtx);
    Move result{Shared_user_input::move.get(ul)};
    return result;
}

}  // namespace chess
