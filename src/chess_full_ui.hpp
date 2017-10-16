#ifndef CHESS_FULL_UI_HPP
#define CHESS_FULL_UI_HPP
#include "chessboard_borders.hpp"
#include "side_pane.hpp"
#include "lower_pane.hpp"
#include "settings_pane.hpp"

#include <signals/slot.hpp>
#include <cppurses/cppurses.hpp>

#include <string>

using namespace cppurses;

class Chess_full_UI : public Horizontal_layout {
   public:
    Chess_full_UI();

    void toggle_logs();

   private:
    Vertical_layout& vl{this->make_child<Vertical_layout>()};
    Widget_stack& stack{vl.make_child<Widget_stack>()};
    Chessboard_with_borders& board{stack.make_page<Chessboard_with_borders>()};
    Settings_pane& settings{stack.make_page<Settings_pane>()};

    Side_pane& side_pane{this->make_child<Side_pane>()};
    Lower_pane& lower_pane{vl.make_child<Lower_pane>()};
    Widget& blank_{vl.make_child<Widget>()};
};

namespace slot {

sig::Slot<void()> toggle_logs(Chess_full_UI& cfui);

sig::Slot<void(const std::string&)> set_player(Chessboard_widget& board,
                                               Side side);

}  // namespace slot

#endif  // CHESS_FULL_UI_HPP
