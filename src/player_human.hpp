#ifndef PLAYER_HUMAN_HPP
#define PLAYER_HUMAN_HPP
#include "player.hpp"
#include "move.hpp"

class Chess_engine;

class Player_human : public Player {
   public:
    Player_human(const Chess_engine& engine) : Player{engine} {}

    Move get_move() override {
        return Move{Position{-1, -1}, Position{-1, -1}};
    }
};

#endif  // PLAYER_HUMAN_HPP
