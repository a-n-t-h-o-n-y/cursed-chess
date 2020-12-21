#ifndef PLAYER_RANDOM_AI_HPP
#define PLAYER_RANDOM_AI_HPP
#include <chrono>
#include <cstddef>
#include <mutex>
#include <random>
#include <thread>

#include "chess_engine.hpp"
#include "player.hpp"
#include "state.hpp"

namespace chess::player {

inline auto random_ai() -> Player
{
    return {
        "Random AI", [](Chess_engine const& engine) -> Move {
            auto const all_pieces = [&e = engine, &s = engine.state()] {
                auto lock = std::lock_guard{e.mtx};
                return s.board.find_all(s.current_side);
            }();
            if (all_pieces.empty())
                return {};

            static auto gen       = std::mt19937{std::random_device{}()};
            auto distribute_piece = std::uniform_int_distribution<std::size_t>{
                0, all_pieces.size() - 1};

            auto moves = Chess_engine::Positions{};
            auto from  = Position{};
            while (moves.empty()) {
                from = all_pieces.at(distribute_piece(gen));
                // TODO player should just be passed this info, so no mutex
                // here.
                auto lock = std::lock_guard{engine.mtx};
                moves     = engine.get_valid_positions(from);
            }

            auto distribute_move =
                std::uniform_int_distribution<std::size_t>{0, moves.size() - 1};
            auto const to = moves.at(distribute_move(gen));

            // TODO this should be built into the UI instead of here.
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return {from, to};
        }};
}

}  // namespace chess::player
#endif  // PLAYER_RANDOM_AI_HPP
