#ifndef MOVE_INPUT_HPP
#define MOVE_INPUT_HPP
#include <string>

#include <signals_light/signal.hpp>

#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/widgets/line_edit.hpp>

#include "move.hpp"

class Move_input : public cppurses::Line_edit {
   public:
    Move_input(cppurses::Glyph_string initial_text);

    // Signals
    sl::Signal<void(Move)> move_requested;
    sl::Signal<void()> reset_requested;

   private:
    void process_action(std::string text);
};
#endif  // MOVE_INPUT_HPP
