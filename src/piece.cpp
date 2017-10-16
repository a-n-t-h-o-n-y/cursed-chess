#include "piece.hpp"

#include <string>

std::string figure_to_text(Figure f) {
    switch (f) {
        case Figure::Bishop:
            return "Bishop";
        case Figure::King:
            return "King";
        case Figure::Knight:
            return "Knight";
        case Figure::Pawn:
            return "Pawn";
        case Figure::Queen:
            return "Queen";
        case Figure::Rook:
            return "Rook";
        case Figure::None:
            return "";
    }
    return "";
}
