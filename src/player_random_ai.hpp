#ifndef PLAYER_RANDOM_AI_HPP
#define PLAYER_RANDOM_AI_HPP
#include "chess_engine.hpp"
#include "player.hpp"

#include <chrono>
#include <cstddef>
#include <random>
#include <thread>

class Player_random_ai : public Player {
   public:
    Player_random_ai(const Chess_engine& engine) : Player{engine} {}

    Move get_move() override {
        Piece filter{Figure::None, engine_.current_side()};
        Chess_engine::Positions all_pieces{engine_.find_positions(filter)};
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
            moves = engine_.get_valid_positions(from);
        }

        std::uniform_int_distribution<std::size_t> dist_move(0,
                                                             moves.size() - 1);
        Position to = moves.at(dist_move(gen));

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return Move{from, to};
    }
};

#endif  // PLAYER_RANDOM_AI_HPP
