#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "chess_engine.hpp"

class Player {
   public:
    Player(Chess_engine* Chess_engine) : Chess_engine_{Chess_engine} {}
    virtual void make_move() = 0;

   protected:
    Chess_engine* Chess_engine_;
};

#endif  // PLAYER_HPP
