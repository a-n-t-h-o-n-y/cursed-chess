#ifndef CHESS_PLAYER_HUMAN_HPP
#define CHESS_PLAYER_HUMAN_HPP
#include "player.hpp"

class Chess_engine;

namespace chess {

class Player_human : public Player {
   public:
    Player_human(const Chess_engine& engine);
    Move get_move() override;
};

}  // namespace chess
#endif  // CHESS_PLAYER_HUMAN_HPP
