#ifndef SETTINGS_PANE_HPP
#define SETTINGS_PANE_HPP
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/painter/trait.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/button.hpp>
#include <cppurses/widget/widgets/checkbox.hpp>
#include <cppurses/widget/widgets/cycle_box.hpp>
#include <cppurses/widget/widgets/fixed_height.hpp>

class Settings_pane : public cppurses::layout::Vertical<> {
   private:
    using Checkbox = cppurses::Labeled_checkbox;

   public:
    Checkbox& show_moves_box = this->make_child<Checkbox>("Show Moves");
    Checkbox& hide_log_box   = this->make_child<Checkbox>("Hide Log");

   public:
    Settings_pane();

   private:
    cppurses::Fixed_height& space_1_{
        this->make_child<cppurses::Fixed_height>()};

   public:
    cppurses::Labeled_cycle_box& black_ai{
        this->make_child<cppurses::Labeled_cycle_box>("Black AI ")};
    cppurses::Labeled_cycle_box& white_ai{
        this->make_child<cppurses::Labeled_cycle_box>("White AI ")};
    cppurses::Labeled_cycle_box& ruleset{
        this->make_child<cppurses::Labeled_cycle_box>("Ruleset  ")};
    cppurses::Button& reset_btn{this->make_child<cppurses::Button>(
        cppurses::Glyph_string{" Reset Game ", cppurses::Trait::Underline})};
    cppurses::Button& return_btn{
        this->make_child<cppurses::Button>("Return to Board")};
};

#endif  // SETTINGS_PANE_HPP
