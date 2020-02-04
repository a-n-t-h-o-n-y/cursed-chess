#include "position.hpp"

#include <functional>
#include <sstream>
#include <string>

std::string to_text(const Position& pos) {
    std::stringstream ss;
    ss << static_cast<char>(pos.column - 1 + 'a') << pos.row;
    return ss.str();
}

bool operator==(const Position& p1, const Position& p2) {
    return p1.row == p2.row and p1.column == p2.column;
}

namespace std {
typename hash<Position>::result_type hash<Position>::operator()(
    const argument_type& position) const noexcept {
    const result_type h1(std::hash<decltype(position.row)>{}(position.row));
    const result_type h2(
        std::hash<decltype(position.column)>{}(position.column));
    return h1 ^ (h2 << 1);
}
}  // namespace std
