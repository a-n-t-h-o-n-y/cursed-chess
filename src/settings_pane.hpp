#ifndef SETTINGS_PANE_HPP
#define SETTINGS_PANE_HPP
#include <cppurses/cppurses.hpp>

using namespace cppurses;

struct Settings_pane : public Vertical_layout {
    Settings_pane();

    Checkbox& show_moves_box{this->make_child<Checkbox>("Show Moves")};
    Checkbox& hide_log_box{this->make_child<Checkbox>("Hide Log")};

   private:
    Blank_height& space_1_{this->make_child<Blank_height>()};

   public:
    Labeled_cycle_box& black_ai{
        this->make_child<Labeled_cycle_box>("Black AI ")};
    Labeled_cycle_box& white_ai{
        this->make_child<Labeled_cycle_box>("White AI ")};
    Labeled_cycle_box& ruleset{
        this->make_child<Labeled_cycle_box>("Ruleset  ")};
    Push_button& reset_btn{this->make_child<Push_button>(
        Glyph_string{" Reset Game ", Attribute::Underline})};
    Push_button& return_btn{this->make_child<Push_button>("Return to Board")};
};

#endif  // SETTINGS_PANE_HPP
