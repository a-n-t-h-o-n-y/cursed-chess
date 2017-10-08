#include "chess_full_ui.hpp"

Chess_full_UI::Chess_full_UI() {
    stack.set_active_page(0);
    stack.width_policy.type(Size_policy::Fixed);
    stack.width_policy.hint(26);
    stack.height_policy.type(Size_policy::Fixed);
    stack.height_policy.hint(10);
}
