#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP
#include <string>

#include "figure.hpp"
#include "position.hpp"
#include "side.hpp"

struct Piece {
    chess::Figure figure;
    chess::Side side;
};

std::string figure_to_text(chess::Figure f);

#endif  // CHESS_PIECE_HPP
