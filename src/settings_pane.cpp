#include "settings_pane.hpp"

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/border.hpp>

using namespace cppurses;

Settings_pane::Settings_pane()
{
    this->brush.set_background(Color::White);
    this->brush.set_foreground(Color::Blue);
    for (auto& descendant : this->Widget::get_descendants()) {
        descendant->brush.set_background(Color::White);
        descendant->brush.set_foreground(Color::Blue);
    }
    return_btn.brush.set_background(Color::Blue);
    return_btn.brush.set_foreground(Color::White);

    this->border.enable();
    this->border.segments.north_east = L'─';
    this->border.segments.north_west = L'╭';
    this->border.segments.south_west = L'╰';
    this->border.segments.south.disable();
    this->border.segments.east.disable();
    this->border.segments.south_east.disable();
}
