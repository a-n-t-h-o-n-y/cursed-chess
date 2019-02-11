#ifndef SETTINGS_PANE_HPP
#define SETTINGS_PANE_HPP
#include <cppurses/painter/attribute.hpp>
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/blank_height.hpp>
#include <cppurses/widget/widgets/checkbox.hpp>
#include <cppurses/widget/widgets/labeled_cycle_box.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

struct Settings_pane : public cppurses::layout::Vertical {
    Settings_pane();

    cppurses::Checkbox& show_moves_box{
        this->make_child<cppurses::Checkbox>("Show Moves")};
    cppurses::Checkbox& hide_log_box{
        this->make_child<cppurses::Checkbox>("Hide Log")};

   private:
    cppurses::Blank_height& space_1_{
        this->make_child<cppurses::Blank_height>()};

   public:
    cppurses::Labeled_cycle_box& black_ai{
        this->make_child<cppurses::Labeled_cycle_box>("Black AI ")};
    cppurses::Labeled_cycle_box& white_ai{
        this->make_child<cppurses::Labeled_cycle_box>("White AI ")};
    cppurses::Labeled_cycle_box& ruleset{
        this->make_child<cppurses::Labeled_cycle_box>("Ruleset  ")};
    cppurses::Push_button& reset_btn{
        this->make_child<cppurses::Push_button>(cppurses::Glyph_string{
            " Reset Game ", cppurses::Attribute::Underline})};
    cppurses::Push_button& return_btn{
        this->make_child<cppurses::Push_button>("Return to Board")};
};
#endif  // SETTINGS_PANE_HPP
