#include "move_input.hpp"

#include <cctype>
#include <string>
#include <utility>

#include "position.hpp"
#include "shared_user_input.hpp"

using namespace cppurses;

namespace {
using namespace chess;

auto text_to_move(std::string const& request_text) -> Move
{
    if (request_text.size() != 4)
        return Move{};
    auto from   = Position{};
    auto to     = Position{};
    from.column = request_text[0] - '`';
    from.row    = request_text[1] - '0';
    to.column   = request_text[2] - '`';
    to.row      = request_text[3] - '0';
    return {from, to};
}

}  // namespace

namespace chess {

Move_input::Move_input(Glyph_string initial_text)
    : Line_edit{std::move(initial_text)}
{
    using namespace cppurses::pipe;

    *this | Trait::Underline | wallpaper(L' ' | Trait::Underline);

    this->clear_on_enter();
    this->edit_finished.connect(
        [this](std::string text) { process_action(text); });
    this->set_validator(
        [](char c) { return !std::ispunct(c) && !std::isspace(c); });
}

void Move_input::process_action(std::string text)
{
    for (char& c : text)
        c = std::tolower(c);
    if (text == "reset")
        reset_request();
    else {
        auto const m = text_to_move(text);
        chess::Shared_user_input::move.set(m);
        move_request(m);
    }
}

}  // namespace chess
