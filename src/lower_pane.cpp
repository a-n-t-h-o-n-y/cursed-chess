#include "lower_pane.hpp"

#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>

#include "chessboard_widget.hpp"
#include "side.hpp"

using namespace cppurses;

Lower_pane::Lower_pane()
{
    this->width_policy.fixed(26);
    this->height_policy.fixed(1);

    for (auto& child : this->get_children()) {
        child.brush.set_background(Color::Dark_blue);
        child.brush.set_foreground(Color::Light_gray);
    }

    // move_input
    move_input.border.enable();
    move_input.border.segments.west = L'│'_g | foreground(Color::Blue);
    move_input.border.segments.east = L'│'_g | foreground(Color::Blue);

    // status
    status.width_policy.fixed(4);
    status.border.enable();
    status.border.segments.disable_all();
    status.border.segments.north.enable();
    status.border.segments.south.enable();
    status.border.segments.west.enable();
    status.border.segments.west = L'╰'_g | foreground(Color::Blue);
}

void Lower_pane::toggle_status(const Chessboard_widget& board)
{
    if (board.current_side() == chess::Side::Black) {
        status.set_contents(" B"_gs | Attribute::Bold |
                            foreground(Color::Black));
        // Glyph_string{" B", Attribute::Bold, foreground(Color::Black)});
    }
    else {
        status.set_contents(
            Glyph_string{" W", Attribute::Bold, foreground(Color::White)});
    }
}

namespace slot {

sig::Slot<void(Move)> toggle_status(Lower_pane& lp,
                                    const Chessboard_widget& board)
{
    sig::Slot<void(Move)> slot{
        [&lp, &board](Move) { lp.toggle_status(board); }};
    slot.track(lp.destroyed);
    slot.track(board.destroyed);
    return slot;
}

}  // namespace slot
