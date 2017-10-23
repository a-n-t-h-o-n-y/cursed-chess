#ifndef CHESS_UI_HPP
#define CHESS_UI_HPP
#include "chessboard_borders.hpp"
#include "lower_pane.hpp"
#include "settings_pane.hpp"
#include "side_pane.hpp"

#include <cppurses/cppurses.hpp>
#include <signals/slot.hpp>

#include <string>

using namespace cppurses;

class Chess_UI : public Horizontal_layout {
   public:
    Chess_UI();

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

sig::Slot<void()> toggle_logs(Chess_UI& cfui);

sig::Slot<void(const std::string&)> set_player(Chessboard_widget& board,
                                               Side side);

// Cyclebox should return signal on add_option(), map of signal and strings
sig::Slot<void(const std::string&)> parse_set_ruleset(Chessboard_widget& board);

}  // namespace slot

#endif  // CHESS_UI_HPP
