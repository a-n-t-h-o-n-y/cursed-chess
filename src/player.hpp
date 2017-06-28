#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "manager.hpp"

class Player {
   public:
    Player(Manager* manager) : manager_{manager} {}
    virtual void make_move() = 0;

   protected:
    Manager* manager_;
};

#endif  // PLAYER_HPP
