#ifndef LOWER_PANE_HPP
#define LOWER_PANE_HPP
#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/color.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/widgets/push_button.hpp>
#include <cppurses/widget/widgets/status_bar.hpp>

#include <signals/slot.hpp>

#include "chessboard_widget.hpp"
#include "move_input.hpp"

struct Lower_pane : public cppurses::layout::Horizontal {
    Lower_pane();

    cppurses::Status_bar& status{this->make_child<cppurses::Status_bar>(
        cppurses::Glyph_string{" W", cppurses::Attribute::Bold,
                               foreground(cppurses::Color::White)})};
    Move_input& move_input{this->make_child<Move_input>("Type Move")};
    cppurses::Push_button& settings_btn{
        this->make_child<cppurses::Push_button>("Settings")};

    void toggle_status(const Chessboard_widget& board);
};

namespace slot {

sig::Slot<void(Move)> toggle_status(Lower_pane& lp,
                                    const Chessboard_widget& board);

}  // namespace slot
#endif  // LOWER_PANE_HPP
