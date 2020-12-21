#ifndef LOWER_PANE_HPP
#define LOWER_PANE_HPP
#include <signals_light/signal.hpp>

#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/painter/trait.hpp>
#include <cppurses/widget/detail/link_lifetimes.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/pipe.hpp>
#include <cppurses/widget/tuple.hpp>
#include <cppurses/widget/widgets/status_bar.hpp>
#include <cppurses/widget/widgets/tile.hpp>
#include <cppurses/widget/widgets/toggle_button.hpp>

#include "chessboard_widget.hpp"
#include "move_input.hpp"
#include "side.hpp"

namespace chess {

using Status = cppurses::
    Tuple<cppurses::layout::Horizontal<>, cppurses::Tile, cppurses::Status_bar>;

class Lower_pane : public cppurses::layout::Horizontal<> {
   public:
    Status& status         = make_child<Status>();
    Move_input& move_input = make_child<Move_input>(L"Type Move");
    cppurses::Toggle_button& btn =
        make_child<cppurses::Toggle_button>(L"Settings", L"Board");

   public:
    sl::Signal<void()>& settings_request = btn.top_pressed;
    sl::Signal<void()>& board_request    = btn.bottom_pressed;
    sl::Signal<void()>& reset_request    = move_input.reset_request;

   public:
    Lower_pane()
    {
        using namespace cppurses::pipe;
        using cppurses::Color;
        using cppurses::Trait;

        *this | fixed_width(26) | fixed_height(1);
        *this | descendants() | bg(Color::Dark_blue) | fg(Color::Light_gray);

        auto constexpr blue_wall = L'│' | fg(Color::Blue);
        move_input | east_west_border() | east_west_walls(blue_wall);

        status | fixed_width(4);
        status.get<0>().set(L'╰' | fg(Color::Blue));
        status.get<1>().update_status({L" W", Trait::Bold, fg(Color::White)});
    }

   public:
    void toggle_status(const Chessboard_widget& board)
    {
        using cppurses::Color;
        using cppurses::Trait;

        auto& side = status.get<1>();
        if (board.current_side() == chess::Side::Black) {
            side.update_status({L" B" | Trait::Bold | fg(Color::Black)});
        }
        else
            side.update_status({L" W" | Trait::Bold | fg(Color::White)});
    }
};

}  // namespace chess

namespace chess::slot {

inline auto toggle_status(Lower_pane& lp, const Chessboard_widget& board)
    -> sl::Slot<void(Move)>
{
    return cppurses::slot::link_lifetimes(
        [&](Move) { lp.toggle_status(board); }, lp, board);
}

}  // namespace chess::slot
#endif  // LOWER_PANE_HPP
