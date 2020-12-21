#include "piece.hpp"

#include <string>

#include "figure.hpp"

namespace chess {

std::string figure_to_text(Figure f)
{
    switch (f) {
        case Figure::Bishop: return "Bishop";
        case Figure::King: return "King";
        case Figure::Knight: return "Knight";
        case Figure::Pawn: return "Pawn";
        case Figure::Queen: return "Queen";
        case Figure::Rook: return "Rook";
    }
    return "";
}

}  // namespace chess
