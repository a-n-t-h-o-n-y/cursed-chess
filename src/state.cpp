#include "state.hpp"
#include "invalid_position.hpp"
#include "piece.hpp"
#include "position.hpp"

#include <utility>

Piece_in_play::Piece_in_play(Piece piece_, Position position_)
    : piece{piece_}, position{position_} {}

// Public Members
State::State() {
    this->reset();
}

Piece& State::at(Position p) {
    return this->find_piece(p);
}

const Piece& State::at(Position p) const {
    return this->find_piece(p);
}

void State::set_current_side(Color side) {
    if (side == Color::None) {
        return;
    }
    current_side_ = side;
}

Color State::current_side() const {
    return current_side_;
}

void State::reset() {
    this->set_current_side(Side::White);
    pieces_.clear();
    pieces_.reserve(64);

    // White
    pieces_.emplace_back(Piece{Figure::Pawn, Color::White}, Position{2, 1});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::White}, Position{2, 2});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::White}, Position{2, 3});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::White}, Position{2, 4});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::White}, Position{2, 5});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::White}, Position{2, 6});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::White}, Position{2, 7});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::White}, Position{2, 8});
    pieces_.emplace_back(Piece{Figure::Rook, Color::White}, Position{1, 1});
    pieces_.emplace_back(Piece{Figure::Knight, Color::White}, Position{1, 2});
    pieces_.emplace_back(Piece{Figure::Bishop, Color::White}, Position{1, 3});
    pieces_.emplace_back(Piece{Figure::King, Color::White}, Position{1, 4});
    pieces_.emplace_back(Piece{Figure::Queen, Color::White}, Position{1, 5});
    pieces_.emplace_back(Piece{Figure::Bishop, Color::White}, Position{1, 6});
    pieces_.emplace_back(Piece{Figure::Knight, Color::White}, Position{1, 7});
    pieces_.emplace_back(Piece{Figure::Rook, Color::White}, Position{1, 8});

    // Black
    pieces_.emplace_back(Piece{Figure::Pawn, Color::Black}, Position{7, 1});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::Black}, Position{7, 2});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::Black}, Position{7, 3});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::Black}, Position{7, 4});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::Black}, Position{7, 5});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::Black}, Position{7, 6});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::Black}, Position{7, 7});
    pieces_.emplace_back(Piece{Figure::Pawn, Color::Black}, Position{7, 8});
    pieces_.emplace_back(Piece{Figure::Rook, Color::Black}, Position{8, 1});
    pieces_.emplace_back(Piece{Figure::Knight, Color::Black}, Position{8, 2});
    pieces_.emplace_back(Piece{Figure::Bishop, Color::Black}, Position{8, 3});
    pieces_.emplace_back(Piece{Figure::King, Color::Black}, Position{8, 4});
    pieces_.emplace_back(Piece{Figure::Queen, Color::Black}, Position{8, 5});
    pieces_.emplace_back(Piece{Figure::Bishop, Color::Black}, Position{8, 6});
    pieces_.emplace_back(Piece{Figure::Knight, Color::Black}, Position{8, 7});
    pieces_.emplace_back(Piece{Figure::Rook, Color::Black}, Position{8, 8});

    // Blank
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{3, 1});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{3, 2});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{3, 3});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{3, 4});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{3, 5});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{3, 6});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{3, 7});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{3, 8});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{4, 1});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{4, 2});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{4, 3});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{4, 4});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{4, 5});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{4, 6});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{4, 7});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{4, 8});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{5, 1});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{5, 2});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{5, 3});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{5, 4});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{5, 5});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{5, 6});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{5, 7});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{5, 8});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{6, 1});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{6, 2});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{6, 3});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{6, 4});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{6, 5});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{6, 6});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{6, 7});
    pieces_.emplace_back(Piece{Figure::None, Color::None}, Position{6, 8});
}

// Free Functions
bool add(State& state, Position position, Piece piece) {
    Piece& space{state.at(position)};
    if (space.figure != Figure::None) {
        return false;
    }
    space = piece;
    return true;
}

void remove(State& state, Position position) {
    Piece& piece{state.at(position)};
    piece.side = Color::None;
    piece.figure = Figure::None;
}

void toggle_current_side(State& state) {
    state.set_current_side(!state.current_side());
}

void make_move(State& state, const Move& move) {
    Piece& piece{state.at(move.from)};
    remove(state, move.to);
    add(state, move.to, piece);
    remove(state, move.from);
    state.move_made(move);
}

// Private Members
Piece& State::find_piece(Position p) {
    auto iter = std::find_if(
        std::begin(pieces_), std::end(pieces_),
        [&p](const Piece_in_play& piece) { return piece.position == p; });
    if (iter == std::end(pieces_)) {
        throw invalid_position(p);
    }
    return iter->piece;
}

const Piece& State::find_piece(Position p) const {
    auto iter = std::find_if(
        std::begin(pieces_), std::end(pieces_),
        [&p](const Piece_in_play& piece) { return piece.position == p; });
    if (iter == std::end(pieces_)) {
        throw invalid_position(p);
    }
    return iter->piece;
}
