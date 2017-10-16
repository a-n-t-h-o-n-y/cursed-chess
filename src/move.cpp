#include "move.hpp"

#include <sstream>
#include <string>

std::string move_to_text(const Move& m) {
    std::stringstream ss;
    ss << static_cast<char>(m.from.column + 'a' - 1);
    ss << m.from.row;
    ss << static_cast<char>(m.to.column + 'a' - 1);
    ss << m.to.row;
    return ss.str();
}
