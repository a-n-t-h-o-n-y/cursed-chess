#ifndef SETTINGS_PANE_HPP
#define SETTINGS_PANE_HPP
#include <array>

#include <signals_light/signal.hpp>

#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/painter/trait.hpp>
#include <cppurses/widget/align.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/pipe.hpp>
#include <cppurses/widget/widgets/checkbox.hpp>
#include <cppurses/widget/widgets/confirm_button.hpp>
#include <cppurses/widget/widgets/cycle_box.hpp>
#include <cppurses/widget/widgets/label.hpp>

#include "player_human.hpp"
#include "player_random_ai.hpp"
#include "side.hpp"

namespace chess {

namespace detail {

auto constexpr make_title = [](auto& w) -> auto&
{
    return w.template make_child<cppurses::HLabel>(
        {L"Settings" | cppurses::Trait::Underline, cppurses::Align::Center});
};

// TODO move out of detail..
class AI_box : public cppurses::layout::Vertical<cppurses::Labeled_cycle_box> {
   public:
    sl::Signal<void(Side, Player)> set;

   private:
    Child_t& black_ai_ = make_child(L"Black AI ");
    Child_t& white_ai_ = make_child(L"White AI ");

   public:
    AI_box()
    {
        auto const players = std::array{
            player::human(),
            player::random_ai(),
        };

        auto const add_players = [&](Side s, cppurses::Cycle_box& cb) {
            for (auto const& p : players)
                cb.add_option(p.name).connect([&] { set.emit(s, p); });
        };

        add_players(Side::Black, black_ai_.cycle_box);
        add_players(Side::White, white_ai_.cycle_box);
    }
};

}  // namespace detail

class Settings_pane : public cppurses::layout::Vertical<> {
   private:
    using Checkbox = cppurses::Labeled_checkbox;
    using Button   = cppurses::Confirm_button;

   private:
    cppurses::HLabel& title_  = detail::make_title(*this);
    Widget& space_1_          = make_child() | cppurses::pipe::fixed_height(1);
    Checkbox& show_moves_box_ = make_child<Checkbox>(L"Show Moves");

   public:
    detail::AI_box& ai_box = make_child<detail::AI_box>();

    // TODO make a ruleset box that has its own signal.
    cppurses::Labeled_cycle_box& ruleset =
        make_child<cppurses::Labeled_cycle_box>(L"Ruleset  ");

   private:
    Widget& space_2_ = make_child() | cppurses::pipe::fixed_height(1);
    Button& reset_btn_ =
        make_child<Button>(L" Reset Game " | cppurses::Trait::Bold);

   public:
    sl::Signal<void()>& reset_request     = reset_btn_.pressed;
    sl::Signal<void()>& toggle_show_moves = show_moves_box_.toggled;

   public:
    Settings_pane()
    {
        using namespace cppurses::pipe;
        using cppurses::Color;

        *this | bg(Color::White) | fg(Color::Blue) | bordered() |
            rounded_corners() | descendants() | bg(Color::White) |
            fg(Color::Blue);

        reset_btn_.confirm_page | children() | bg(Color::Blue) |
            fg(Color::White);
    }
};

}  // namespace chess
#endif  // SETTINGS_PANE_HPP
