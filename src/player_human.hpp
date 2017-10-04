#ifndef PLAYER_HUMAN_HPP
#define PLAYER_HUMAN_HPP

#include "chess_engine.hpp"
#include "player.hpp"

class Player_human : public Player {
   public:
    Player_human(Chess_engine* Chess_engine) : Player(Chess_engine) {}
    void make_move() override {}
};

#endif  // PLAYER_HUMAN_HPP
