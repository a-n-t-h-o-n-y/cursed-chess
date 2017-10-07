#ifndef INVALID_POSITION_HPP
#define INVALID_POSITION_HPP
#include "position.hpp"

#include <stdexcept>

class Invalid_position : public std::out_of_range {
   public:
    Invalid_position(Position p);
};

#endif  // INVALID_POSITION_HPP
