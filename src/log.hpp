#ifndef LOG_HPP
#define LOG_HPP
#include <cppurses/cppurses.hpp>

using namespace cppurses;

class Log : public Text_display {
   public:
    Log();

    sig::Slot<void(Glyph_string)> append_message;

   protected:
    bool mouse_press_event(Mouse_button button,
                           std::size_t global_x,
                           std::size_t global_y,
                           std::size_t local_x,
                           std::size_t local_y,
                           std::uint8_t device_id) override;

    bool key_press_event(Key key, char symbol) override;

   private:
    Centered_title& title_{this->make_child<Centered_title>("ᴸᵒᵍ")};
    Text_display& log_{this->make_child<Text_display>()};
    Text_display& status_bar_{this->make_child<Text_display>("ᵂʰᶦᵗᵉ'ˢ ᵐᵒᵛᵉ")};
    bool scroll_{false};

   // public:
    // sig::Slot<void(Glyph_string)>& set_status{status_bar_.set_text};
    // sig::Slot<void(Glyph_string)> set_status{
    //     cppurses::slot::set_text(status_bar_)};
};

#endif  // LOG_HPP
