#ifndef PLAYER_HUMAN_HPP
#define PLAYER_HUMAN_HPP

#include "manager.hpp"
#include "player.hpp"

class Player_human : public Player {
   public:
    Player_human(Manager* manager) : Player(manager) {}
    void make_move() override {}
};

#endif  // PLAYER_HUMAN_HPP
