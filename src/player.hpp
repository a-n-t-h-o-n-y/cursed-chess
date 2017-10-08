#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "move.hpp"

class Chess_engine;

class Player {
   public:
    Player(const Chess_engine& engine) : engine_{engine} {}
    virtual Move get_move() = 0;

   protected:
    const Chess_engine& engine_;
};

#endif  // PLAYER_HPP
