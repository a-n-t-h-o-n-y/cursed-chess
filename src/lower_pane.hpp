#ifndef LOWER_PANE_HPP
#define LOWER_PANE_HPP
#include "move_input.hpp"
#include "chessboard_widget.hpp"

#include <cppurses/cppurses.hpp>
#include <signals/slot.hpp>

using namespace cppurses;

struct Lower_pane : public Horizontal_layout {
    Lower_pane();

    Status_bar& status{this->make_child<Status_bar>(
        Glyph_string{" W", Attribute::Bold, foreground(Color::White)})};
    Move_input& move_input{this->make_child<Move_input>("Type Move")};
    Push_button& settings_btn{this->make_child<Push_button>("Settings")};

    void toggle_status(const Chessboard_widget& board);
};

namespace slot {

sig::Slot<void(Move)> toggle_status(Lower_pane& lp,
                                    const Chessboard_widget& board);

}  // namespace slot

#endif  // LOWER_PANE_HPP
