#ifndef PLAYER_RANDOM_AI_HPP
#define PLAYER_RANDOM_AI_HPP
#include <chrono>
#include <cstddef>
#include <mutex>
#include <random>
#include <thread>

#include "chess_engine.hpp"
#include "figure.hpp"
#include "player.hpp"
#include "state.hpp"

class Player_random_ai : public Player {
   public:
    Player_random_ai(const Chess_engine& engine) : Player{engine} {}

    Move get_move() override {
        Chess_engine::Positions all_pieces;
        {
            // TODO this mtx is temporary... bad design here.
            std::lock_guard<std::recursive_mutex> lock{engine_.mtx};
            all_pieces =
                engine_.state().board.find_all(engine_.state().current_side);
        }
        if (all_pieces.empty()) {
            return Move{};
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> dist_piece(
            0, all_pieces.size() - 1);

        Chess_engine::Positions moves;
        Position from;
        while (moves.empty()) {
            from = all_pieces.at(dist_piece(gen));
            // TODO player should just be passed this info, so no mutex here.
            std::lock_guard<std::recursive_mutex> lock{engine_.mtx};
            moves = engine_.get_valid_positions(from);
        }

        std::uniform_int_distribution<std::size_t> dist_move(0,
                                                             moves.size() - 1);
        Position to = moves.at(dist_move(gen));

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return Move{from, to};
    }
};

#endif  // PLAYER_RANDOM_AI_HPP
