#ifndef PLAYER_RANDOM_AI_HPP
#define PLAYER_RANDOM_AI_HPP
#include "chess_engine.hpp"
#include "player.hpp"

#include <chrono>
#include <random>
#include <thread>

class Player_random_ai : public Player {
   public:
    Player_random_ai(const Chess_engine& engine) : Player{engine} {}

    Move get_move() override {
        auto moves = engine_.get_all_potential_moves(engine_.current_side());
        if (moves.empty()) {
            return;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, moves.size() - 1);
        auto& move_pair = moves.at(dist(gen));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return Move{move_pair.first, move_pair.second};
    }
};

#endif  // PLAYER_RANDOM_AI_HPP
