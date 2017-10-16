#ifndef SETTINGS_PANE_HPP
#define SETTINGS_PANE_HPP
#include <cppurses/cppurses.hpp>

using namespace cppurses;

struct Settings_pane : public Vertical_layout {
    Settings_pane();

    // private:
    // Label& title_{this->make_child<Label>("Settings")};

   public:
    Checkbox& show_moves_box{this->make_child<Checkbox>("Show Moves")};
    Checkbox& hide_log_box{this->make_child<Checkbox>("Hide Log")};

   private:
    Blank_height& space_1_{this->make_child<Blank_height>()};

   public:
    Cycle_box& black_ai{this->make_child<Cycle_box>("Black AI ")};
    Cycle_box& white_ai{this->make_child<Cycle_box>("White AI ")};

   private:
    // Blank_height& space_2_{this->make_child<Blank_height>()};

   public:
    Cycle_box& ruleset{this->make_child<Cycle_box>("Ruleset  ")};
    Push_button& reset_btn{this->make_child<Push_button>(
        Glyph_string{" Reset Game ", Attribute::Underline})};

    // private:
    // Blank_height& space_3_{this->make_child<Blank_height>()};

   public:
    Push_button& return_btn{this->make_child<Push_button>("Return to Board")};
};

#endif  // SETTINGS_PANE_HPP
