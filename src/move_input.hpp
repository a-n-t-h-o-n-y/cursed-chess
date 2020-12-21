#ifndef MOVE_INPUT_HPP
#define MOVE_INPUT_HPP
#include <string>

#include <signals_light/signal.hpp>

#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/widgets/line_edit.hpp>

#include "move.hpp"

namespace chess {

class Move_input : public cppurses::Line_edit {
   public:
    // TODO nothing uses move_request right now.. hook it up.
    sl::Signal<void(Move)> move_request;
    sl::Signal<void()> reset_request;

   public:
    Move_input(cppurses::Glyph_string initial_text);

   private:
    void process_action(std::string text);
};

}  // namespace chess
#endif  // MOVE_INPUT_HPP
