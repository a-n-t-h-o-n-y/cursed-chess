#include "log.hpp"

#include <cppurses/cppurses.hpp>
#include <signals/signals.hpp>

Log::Log() {
    this->focus_policy = Focus_policy::Strong;
    this->width_policy.type(Size_policy::Maximum);
    this->width_policy.hint(21);
    this->height_policy.type(Size_policy::Fixed);
    this->height_policy.hint(12);

    set_background(*this, Color::White);
    set_foreground(*this, Color::Dark_gray);

    set_foreground(title_, Color::Black);
    set_background(title_, Color::Light_gray);

    set_foreground(log_, Color::White);
    set_background(log_, Color::Gray);

    status_bar_.height_policy.type(Size_policy::Fixed);
    status_bar_.height_policy.hint(1);
    set_background(status_bar_, Color::White);
    set_foreground(status_bar_, Color::Gray);
    status_bar_.disable_word_wrap();

    // Slot
    append_message.track(this->destroyed);
    append_message = [this](Glyph_string message) {
        auto conts = log_.contents();
        if (!scroll_) {
            if (std::count(std::begin(conts), std::end(conts), '\n') > 10) {
                scroll_ = true;
            }
        }
        log_.append(message);
        if (scroll_ && !conts.empty() && conts.back() == '\n') {
            log_.scroll_down();
        }
    };
}

bool Log::mouse_press_event(Mouse_button button,
                            std::size_t global_x,
                            std::size_t global_y,
                            std::size_t local_x,
                            std::size_t local_y,
                            std::uint8_t device_id) {
    if (button == Mouse_button::ScrollUp) {
        log_.scroll_up();
    } else if (button == Mouse_button::ScrollDown) {
        log_.scroll_down();
    }
    return Vertical_layout::mouse_press_event(button, global_x, global_y,
                                              local_x, local_y, device_id);
}

bool Log::key_press_event(Key key, char symbol) {
    if (key == Key::Arrow_up || key == Key::k) {
        log_.scroll_up();
    } else if (key == Key::Arrow_down || key == Key::j) {
        log_.scroll_down();
    }
    return Vertical_layout::key_press_event(key, symbol);
}
