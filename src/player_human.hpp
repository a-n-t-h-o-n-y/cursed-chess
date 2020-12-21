#ifndef CHESS_PLAYER_HUMAN_HPP
#define CHESS_PLAYER_HUMAN_HPP
#include <mutex>

#include "move.hpp"
#include "player.hpp"
#include "shared_user_input.hpp"

namespace chess {
class Chess_engine;
}  // namespace chess

namespace chess::player {

inline auto human() -> Player
{
    return {"Human", [](Chess_engine const&) {
                auto lock = std::unique_lock{Shared_user_input::move.mtx};
                return Shared_user_input::move.get(lock);
            }};
}

}  // namespace chess::player
#endif  // CHESS_PLAYER_HUMAN_HPP
