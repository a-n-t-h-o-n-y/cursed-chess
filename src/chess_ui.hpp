#ifndef CHESS_UI_HPP
#define CHESS_UI_HPP
#include <string>

#include <cppurses/cppurses.hpp>
#include <signals/slot.hpp>

#include "chessboard_borders.hpp"
#include "lower_pane.hpp"
#include "settings_pane.hpp"
#include "side_pane.hpp"

using namespace cppurses;

class Left_side : public Vertical_layout {
   public:
    Left_side();

    void enable(bool enable = true,
                bool post_child_polished_event = true) override;

    Widget_stack& stack{this->make_child<Widget_stack>()};
    Chessboard_with_borders& board{stack.make_page<Chessboard_with_borders>()};
    Settings_pane& settings{stack.make_page<Settings_pane>()};
    Lower_pane& lower_pane{this->make_child<Lower_pane>()};

    bool lower_pane_enabled{false};
};

class Chess_UI : public Horizontal_layout {
   public:
    Chess_UI();

    void toggle_logs();

    void enable(bool enable = true,
                bool post_child_polished_event = true) override;

   private:
    Left_side& left_side{this->make_child<Left_side>()};
    Side_pane& right_side{this->make_child<Side_pane>()};
};

namespace slot {

sig::Slot<void()> toggle_logs(Chess_UI& cfui);

}  // namespace slot

#endif  // CHESS_UI_HPP
