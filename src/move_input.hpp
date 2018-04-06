#ifndef MOVE_INPUT_HPP
#define MOVE_INPUT_HPP
#include <cppurses/cppurses.hpp>
#include <signals/signals.hpp>

#include "move.hpp"

using namespace cppurses;

class Move_input : public Line_edit {
   public:
    Move_input(Glyph_string initial_text);

    // Signals
    sig::Signal<void(Move)> move_requested;
    sig::Signal<void()> reset_requested;

   private:
    void process_action(std::string text);
};

#endif  // MOVE_INPUT_HPP
