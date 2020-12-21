#ifndef BOARD_POSITION_HPP
#define BOARD_POSITION_HPP
#include <functional>
#include <string>

namespace chess {

struct Position {
    int row;     // Bottom to Top[1,8]
    int column;  // Left to Right[1,8]
};

auto to_text(Position pos) -> std::string;

auto operator==(Position p1, Position p2) -> bool;

}  // namespace chess

/// Custom specialization of std::hash for Position.
namespace std {
template <>
struct hash<chess::Position> {
    using argument_type = chess::Position;
    using result_type   = std::size_t;
    result_type operator()(argument_type position) const noexcept;
};
}  // namespace std
#endif  // BOARD_POSITION_HPP
