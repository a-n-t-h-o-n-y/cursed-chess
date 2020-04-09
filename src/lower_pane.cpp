#include "lower_pane.hpp"

#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/widget/pipe.hpp>

#include "chessboard_widget.hpp"
#include "side.hpp"

using namespace cppurses;
using namespace cppurses::pipe;

Lower_pane::Lower_pane()
{
    *this | fixed_width(26) | fixed_height(1);
    *this | descendants() | bg(Color::Dark_blue) | fg(Color::Light_gray);

    auto const blue_wall = L'│'_g | foreground(Color::Blue);
    move_input | east_west_border() | east_west_walls(blue_wall);

    status | fixed_width(4);
    status.get<0>().set(L'╰'_g | foreground(Color::Blue));
    status.get<1>().update_status(
        {" W", Attribute::Bold, foreground(Color::White)});
}

void Lower_pane::toggle_status(const Chessboard_widget& board)
{
    auto& side = status.get<1>();
    if (board.current_side() == chess::Side::Black) {
        side.update_status(
            {L" B" | Attribute::Bold | foreground(Color::Black)});
    }
    else
        side.update_status({" W", Attribute::Bold, foreground(Color::White)});
}

namespace slot {

sig::Slot<void(Move)> toggle_status(Lower_pane& lp,
                                    const Chessboard_widget& board)
{
    auto slot = sig::Slot<void(Move)>{[&](Move) { lp.toggle_status(board); }};
    slot.track(lp.destroyed);
    slot.track(board.destroyed);
    return slot;
}

}  // namespace slot
