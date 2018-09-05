#ifndef BOARD_POSITION_HPP
#define BOARD_POSITION_HPP
#include <functional>
#include <string>

struct Position {
    int row;     // Bottom to Top[1,8]
    int column;  // Left to Right[1,8]
};

std::string to_text(const Position& pos);

bool operator==(const Position& p1, const Position& p2);

/// Custom specialization of std::hash for Position.
namespace std {
template <>
struct hash<Position> {
    using argument_type = Position;
    using result_type = std::size_t;
    result_type operator()(const argument_type& position) const noexcept;
};
}  // namespace std
#endif  // BOARD_POSITION_HPP
