#ifndef MOVE_INPUT_HPP
#define MOVE_INPUT_HPP
#include <string>

#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/widgets/line_edit.hpp>

#include <signals/signals.hpp>

#include "move.hpp"

class Move_input : public cppurses::Line_edit {
   public:
    Move_input(cppurses::Glyph_string initial_text);

    // Signals
    sig::Signal<void(Move)> move_requested;
    sig::Signal<void()> reset_requested;

   private:
    void process_action(std::string text);
};
#endif  // MOVE_INPUT_HPP
