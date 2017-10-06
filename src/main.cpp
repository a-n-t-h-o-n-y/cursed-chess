#include <algorithm>
#include <cctype>
#include <cppurses/cppurses.hpp>
#include <signals/signals.hpp>
#include "chessboard_widget.hpp"
#include "piece.hpp"
#include "player.hpp"

using namespace cppurses;

class Centered_title : public Widget {
   public:
    Centered_title(Glyph_string title) : title_{title} {
        this->height_policy.type(Size_policy::Fixed);
        this->height_policy.hint(1);
    }

    bool paint_event() override {
        Painter p{this};
        auto sz = static_cast<int>(title_.size());
        auto start = (static_cast<int>(this->width()) - sz) / 2;
        if (start < 0) {
            start = 0;
        }
        p.put(title_, start, 0);
        return Widget::paint_event();
    }

   private:
    Glyph_string title_;
};

class Command_line_input : public Textbox {
   public:
    Command_line_input() : Textbox("ᵗʸᵖᵉ 'ʰᵉᶫᵖ'") {
        set_background(*this, Color::White);
        set_foreground(*this, Color::Gray);
        this->height_policy.type(Size_policy::Fixed);
        this->height_policy.hint(1);
        this->disable_word_wrap();
    }

    bool key_press_event(Key key, char symbol) override {
        if (key == Key::Enter) {
            command_given(this->contents());
            std::string command_text = this->contents().str();
            if (command_text == "help") {
                command_given(
                    "~ᴹᵃᵏᵉ ᵐᵒᵛᵉ~\n⁽ᵖᵒˢ¹⁾⁽ᵖᵒˢ²⁾\nᵉˣ⁾ ᵉ²ᵉ⁴\n~ˢᵉᵗ ᵖᶫᵃʸᵉʳ ᴬᴵ~\nˢᵉᵗ "
                    "⁽ˢᶦᵈᵉ⁾ ⁽ᵃᶦ⁾\nᵉˣ⁾\nˢᵉᵗ ʷʰᶦᵗᵉ ᵃᶦ_ʳᵃᶰᵈᵒᵐ");
            }
            if (command_text.size() > 14) {
                if (command_text.substr(0, 4) == "set ") {
                    Side player;
                    if (command_text.substr(4, 5) == "black") {
                        player = Side::Black;
                    } else if (command_text.substr(4, 5) == "white") {
                        player = Side::White;
                    }
                    auto ai = command_text.substr(10, 5);
                    if (ai == "human") {
                        player_change(player, "human");

                    } else if (ai == "ai_ra") {
                        player_change(player, "ai_random");
                    }
                }
            }
            if (command_text.size() > 3) {
                Position pos1{command_text[1] - '0',
                              std::tolower(command_text[0]) - 'a' + 1};
                Position pos2{command_text[3] - '0',
                              std::tolower(command_text[2]) - 'a' + 1};
                move_made(pos1, pos2);
            }
            this->clear();
            move_cursor(*this, 0, 0);
            return true;
        }
        return Textbox::key_press_event(key, symbol);
    }

    bool mouse_press_event(Mouse_button button,
                           std::size_t global_x,
                           std::size_t global_y,
                           std::size_t local_x,
                           std::size_t local_y,
                           std::uint8_t device_id) override {
        if (first_click_) {
            first_click_ = false;
            this->clear();
        }
        return Textbox::mouse_press_event(button, global_x, global_y, local_x,
                                          local_y, device_id);
    }

    sig::Signal<void(Position, Position)> move_made;
    sig::Signal<void(Side, std::string)> player_change;
    sig::Signal<void(Glyph_string)> command_given;

   private:
    bool first_click_{true};
};

class Command_line_widget : public Vertical_layout {
   public:
    Command_line_widget() {
        this->width_policy.type(Size_policy::Maximum);
        this->width_policy.hint(21);
        this->height_policy.type(Size_policy::Fixed);
        this->height_policy.hint(12);

        set_foreground(title_, Color::Black);
        set_background(title_, Color::Light_gray);

        set_foreground(log_, Color::White);
        set_background(log_, Color::Gray);
        log_.disable_word_wrap();

        append_message.track(this->destroyed);
        append_message = [this](Glyph_string message) {
            auto conts = log_.contents();
            if (!scroll_) {
                if (std::count(std::begin(conts), std::end(conts), '\n') > 10) {
                    scroll_ = true;
                }
            }
            log_.append(message + "\n");
            if (scroll_ && !conts.empty() && conts.back() == '\n') {
                log_.scroll_down();
            }
        };
        input_.command_given.connect(append_message);
    }

    bool mouse_press_event(Mouse_button button,
                           std::size_t global_x,
                           std::size_t global_y,
                           std::size_t local_x,
                           std::size_t local_y,
                           std::uint8_t device_id) override {
        if (button == Mouse_button::ScrollUp) {
            log_.scroll_up();
        } else if (button == Mouse_button::ScrollDown) {
            log_.scroll_down();
        }
        return Vertical_layout::mouse_press_event(button, global_x, global_y,
                                                  local_x, local_y, device_id);
    }

   private:
    Centered_title& title_{this->make_child<Centered_title>("ᶜᵒᵐᵐᵃᶰᵈ ᴸᶦᶰᵉ")};
    Text_display& log_{this->make_child<Text_display>()};
    Command_line_input& input_{this->make_child<Command_line_input>()};
    bool scroll_{false};
    sig::Slot<void(Glyph_string)> append_message;

   public:
    sig::Signal<void(Position, Position)>& move_made{input_.move_made};
    sig::Signal<void(Side, std::string)>& player_change{input_.player_change};
};

class Rows_listing : public Text_display {
   public:
    Rows_listing() : Text_display{"⁸⁷⁶⁵⁴³²¹"} {
        this->width_policy.type(Size_policy::Fixed);
        this->width_policy.hint(1);
        this->height_policy.type(Size_policy::Fixed);
        this->height_policy.hint(8);
        set_foreground(*this, Color::Gray);
        set_background(*this, Color::White);
        this->disable_word_wrap();
    }
};

class Columns_listing : public Text_display {
   public:
    Columns_listing() : Text_display{"  ᵃ  ᵇ  ᶜ  ᵈ  ᵉ  ᶠ  ᵍ  ʰ"} {
        this->width_policy.type(Size_policy::Fixed);
        this->width_policy.hint(26);
        this->height_policy.type(Size_policy::Fixed);
        this->height_policy.hint(1);
        set_foreground(*this, Color::Gray);
        set_background(*this, Color::White);
        this->disable_word_wrap();
    }
};

class Log_widget : public Vertical_layout {
   public:
    Log_widget() {
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

    bool mouse_press_event(Mouse_button button,
                           std::size_t global_x,
                           std::size_t global_y,
                           std::size_t local_x,
                           std::size_t local_y,
                           std::uint8_t device_id) override {
        if (button == Mouse_button::ScrollUp) {
            log_.scroll_up();
        } else if (button == Mouse_button::ScrollDown) {
            log_.scroll_down();
        }
        return Vertical_layout::mouse_press_event(button, global_x, global_y,
                                                  local_x, local_y, device_id);
    }

    bool key_press_event(Key key, char symbol) override {
        if (key == Key::Arrow_up || key == Key::k) {
            log_.scroll_up();
        } else if (key == Key::Arrow_down || key == Key::j) {
            log_.scroll_down();
        }
        return Vertical_layout::key_press_event(key, symbol);
    }

    sig::Slot<void(Glyph_string)> append_message;

   private:
    Centered_title& title_{this->make_child<Centered_title>("ᴸᵒᵍ")};
    Text_display& log_{this->make_child<Text_display>()};
    Text_display& status_bar_{this->make_child<Text_display>("ᵂʰᶦᵗᵉ'ˢ ᵐᵒᵛᵉ")};
    bool scroll_{false};

   public:
    // sig::Slot<void(Glyph_string)>& set_status{status_bar_.set_text};
    sig::Slot<void(Glyph_string)> set_status{
        cppurses::slot::set_text(status_bar_)};
};

class Settings : public Horizontal_layout {
   public:
    Settings() {
        this->height_policy.type(Size_policy::Preferred);
        this->height_policy.hint(0);

        vert_layout1_.width_policy.type(Size_policy::Fixed);
        vert_layout1_.width_policy.hint(12);

        vert_layout2_.width_policy.type(Size_policy::Fixed);
        vert_layout2_.width_policy.hint(13);

        reset_button_.height_policy.type(Size_policy::Fixed);
        reset_button_.height_policy.hint(1);
        set_background(reset_button_, Color::Dark_gray);
        set_foreground(reset_button_, Color::White);

        exit_button_.height_policy.type(Size_policy::Fixed);
        exit_button_.height_policy.hint(1);
        set_background(exit_button_, Color::Light_gray);
        set_foreground(exit_button_, Color::White);

        set_background(placeholder_checkbox_, Color::Dark_gray);
        set_foreground(placeholder_checkbox_, Color::White);

        set_background(moves_checkbox_, Color::Light_gray);
        set_foreground(moves_checkbox_, Color::White);

        seperator_.width_policy.type(Size_policy::Fixed);
        seperator_.width_policy.hint(1);
        seperator_.height_policy.type(Size_policy::Fixed);
        seperator_.height_policy.hint(2);
        set_background(seperator_, Color::Black);
        set_foreground(seperator_, Color::Light_gray);
        Glyph a{"├", background(Color::Dark_gray), foreground(Color::White)};
        Glyph b{"├", background(Color::Light_gray), foreground(Color::White)};
        Glyph_string bar;
        bar.append(a).append(b);
        seperator_.set_text(bar);

        // Slot
        exit_game.track(this->destroyed);
        exit_game = [this](auto* button) {
            if (exit_first_click_) {
                exit_first_click_ = false;
                button->set_text("ᴿ ᵘ ˢᵘʳᵉ?");
            } else {
                System::quit();
            }
        };
        exit_game_sig.connect(exit_game);
    }

   private:
    Vertical_layout& vert_layout1_{this->make_child<Vertical_layout>()};
    Push_button& reset_button_{
        vert_layout1_.make_child<Push_button>("ᴿᵉˢᵉᵗ ᴳᵃᵐᵉ")};
    Text_display& seperator_{this->make_child<Text_display>()};
    Push_button& exit_button_{vert_layout1_.make_child<Push_button>("ᴱˣᶦᵗ")};
    Vertical_layout& vert_layout2_{this->make_child<Vertical_layout>()};
    Checkbox& placeholder_checkbox_{
        vert_layout2_.make_child<Checkbox>("ˢᵒᵐᵉ ᴼᵖᵗᶦᵒᶰ", 2)};
    Checkbox& moves_checkbox_{
        vert_layout2_.make_child<Checkbox>("ˢʰᵒʷ ᴹᵒᵛᵉˢ", 2)};

    sig::Signal<void(Push_button*)>& exit_game_sig = exit_button_.clicked_w_ref;
    sig::Slot<void(Push_button*)> exit_game;
    bool exit_first_click_{true};

   public:
    sig::Signal<void()>& show_moves_toggled = moves_checkbox_.toggled;
    sig::Signal<void(Push_button*)>& call_reset_game =
        reset_button_.clicked_w_ref;
};

int main() {
    System s;

    Vertical_layout outer;
    auto& hl = outer.make_child<Horizontal_layout>();
    auto& command_line = hl.make_child<Command_line_widget>();

    auto& vl = hl.make_child<Vertical_layout>();
    vl.width_policy.type(Size_policy::Fixed);
    vl.width_policy.hint(26);
    vl.make_child<Columns_listing>();

    auto& top_layout = vl.make_child<Horizontal_layout>();
    top_layout.height_policy.type(Size_policy::Fixed);
    top_layout.height_policy.hint(8);

    top_layout.make_child<Rows_listing>();
    auto& chessboard = top_layout.make_child<Chessboard_widget>();
    top_layout.make_child<Rows_listing>();
    vl.make_child<Columns_listing>();
    auto& settings = vl.make_child<Settings>();

    auto& log = hl.make_child<Log_widget>();

    command_line.move_made.connect(::slot::make_move(chessboard));
    command_line.player_change.connect(::slot::change_ai(chessboard));
    settings.show_moves_toggled.connect(::slot::toggle_show_moves(chessboard));
    settings.call_reset_game.connect(::slot::reset_game(chessboard));
    chessboard.move_message.connect(log.append_message);
    chessboard.status_message.connect(log.set_status);

    s.set_head(&hl);

    return s.run();
}
