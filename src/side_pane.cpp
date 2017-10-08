#include "side_pane.hpp"

Move_settings_bar::Move_settings_bar() {
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(1);
}

Side_pane::Side_pane() {}
