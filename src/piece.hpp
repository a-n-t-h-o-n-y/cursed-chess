#ifndef PIECE_HPP
#define PIECE_HPP
#include "position.hpp"

#include <string>

enum class Side { None, White, Black };

enum class Figure { None, Bishop, King, Knight, Pawn, Queen, Rook };

struct Piece {
    Figure figure;
    Side side;
};

std::string figure_to_text(Figure f);

#endif  // PIECE_HPP
