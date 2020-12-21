#include "position.hpp"

#include <functional>
#include <sstream>
#include <string>

namespace chess {

auto to_text(Position pos) -> std::string
{
    auto ss = std::stringstream{};
    ss << static_cast<char>(pos.column - 1 + 'a') << pos.row;
    return ss.str();
}

auto operator==(Position p1, Position p2) -> bool
{
    return p1.row == p2.row and p1.column == p2.column;
}

}  // namespace chess

namespace std {

auto hash<chess::Position>::operator()(argument_type position) const noexcept ->
    typename hash<chess::Position>::result_type
{
    auto const h1 = std::hash<decltype(position.row)>{}(position.row);
    auto const h2 = std::hash<decltype(position.column)>{}(position.column);
    return h1 ^ (h2 << 1);
}

}  // namespace std
