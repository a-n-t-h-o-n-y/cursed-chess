#include "invalid_position.hpp"
#include "position.hpp"
#include <string>
#include <sstream>

namespace {

std::string to_string(Position p) {
    std::stringstream ss;
    ss << "row: " << std::to_string(p.row) << '\n';
    ss << "column: " << std::to_string(p.column) << std::endl;
    return ss.str();
}

}  // namespace

Invalid_position::Invalid_position(Position p) : out_of_range{to_string(p)} {}
