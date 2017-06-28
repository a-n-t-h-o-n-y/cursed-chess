#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "piece.hpp"
#include "piece_king.hpp"
#include "piece_pawn.hpp"
#include "piece_rook.hpp"
#include "piece_queen.hpp"
#include "piece_bishop.hpp"
#include "piece_knight.hpp"
#include "position.hpp"
#include <signals/signals.hpp>
#include <algorithm>
#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>

class Manager {
   public:
    Manager() { this->reset_board(); }
    std::string status_string() const {
        // if any of current players pieces have moves that touch the opponents
        // king, then the message is "check"

        // if any opponent is missing a king, then the game is checkmate

        // otherwise list piece counts? list anyways as well?
        // what other things in a status string?
        return std::string{};
    }

    bool game_over() const {
        int count{0};
        for (const auto& piece : pieces_) {
            if (piece->get_display() == "♚") {
                ++count;
            }
        }
        if (count != 2) {
            return true;
        }
        return false;
    }

    void reset_game() {
        this->reset_board();
        move_count_ = 0;
        this->status_message_sig("ᴳᵃᵐᵉ ʳᵉˢᵉᵗ\n");
    }

    std::vector<Position> get_moves(Position pos) {
        auto* piece = find_piece(pos);
        if (piece == nullptr) {
            return std::vector<Position>{};
        }
        piece->update_valid_moves(
            [this](Position p) { return this->get_side(p); });
        auto moves = piece->valid_moves();
        auto new_end = std::remove_if(
            std::begin(moves), std::end(moves), [](const auto& pos) {
                return pos.row < 1 || pos.row > 8 || pos.column < 1 ||
                       pos.column > 8;
            });
        moves.erase(new_end, std::end(moves));
        return moves;
    }

    std::vector<std::pair<Position, Position>> get_all_potential_moves(
        Side side) {
        std::vector<std::pair<Position, Position>> moves;
        for (const auto& piece : pieces_) {
            if (piece->get_side() == side) {
                auto to_pos = this->get_moves(piece->get_position());
                for (const auto& pos : to_pos) {
                    moves.push_back(std::make_pair(piece->get_position(), pos));
                }
            }
        }
        return moves;
    }

    Side current_side() const { return current_side_; }

    bool make_move(Position pos1, Position pos2) {
        auto* piece = this->find_piece(pos1);
        if (piece == nullptr) {
            return false;
        }
        if (piece->get_side() != current_side_) {
            this->status_message_sig("ᴺᵒᵗ ʸᵒᵘʳ ᵖᶦᵉᶜᵉ\n");
            return false;
        }
        if (!this->check_move(pos1, pos2)) {
            this->status_message_sig("ᴵᶰᵛᵃᶫᶦᵈ ᵐᵒᵛᵉ\n");
            return false;
        }
        // Move is valid
        if (piece->get_side() == Side::White) {
            ++move_count_;
        }
        report_move(pos1, pos2);
        this->remove_piece(pos2);
        piece->set_position(pos2);
        if (this->game_over()) {
            if (this->current_side() == Side::White) {
                this->move_message_sig("¹⁻⁰\n");
            } else {
                this->move_message_sig("⁰⁻¹\n");
            }
        }
        this->flip_side();
        move_made_sig();
        return true;
    }

    const std::vector<std::unique_ptr<Piece>>& get_pieces() const {
        return pieces_;
    }

    // Signal
    sig::Signal<void(std::string)> move_message_sig;
    sig::Signal<void(std::string)> status_message_sig;
    sig::Signal<void()> move_made_sig;

    friend class Chess_widget;

   private:
    Side current_side_{Side::White};
    std::vector<std::unique_ptr<Piece>> pieces_;
    int move_count_{0};

    void flip_side() {
        if (current_side_ == Side::Black) {
            current_side_ = Side::White;
            status_message_sig("ᵂʰᶦᵗᵉ'ˢ ᵐᵒᵛᵉ");
        } else {
            current_side_ = Side::Black;
            status_message_sig("ᴮᶫᵃᶜᵏ'ˢ ᵐᵒᵛᵉ");
        }
    }

    void remove_piece(Position p) {
        auto iter = std::find_if(
            std::begin(pieces_), std::end(pieces_),
            [p](const auto& uptr) { return uptr->get_position() == p; });
        if (iter != std::end(pieces_)) {
            pieces_.erase(iter);
        }
    }

    Side get_side(Position p) const {
        auto* piece = this->find_piece(p);
        if (piece != nullptr) {
            return piece->get_side();
        }
        return Side::None;
    }

    Piece* find_piece(Position p) const {
        for (const auto& piece : pieces_) {
            if (piece->get_position() == p) {
                return piece.get();
            }
        }
        return nullptr;
    }

    bool check_move(Position pos1, Position pos2) {
        auto* piece = find_piece(pos1);
        if (piece == nullptr) {
            return false;
        }
        piece->update_valid_moves(
            [this](Position p) { return this->get_side(p); });
        return piece->can_move(pos2);
    }

    static std::string int_to_tiny(const std::string& number) {
        std::string result;
        std::map<char, std::string> tinys{
            {'0', "⁰"}, {'1', "¹"}, {'2', "²"}, {'3', "³"}, {'4', "⁴"},
            {'5', "⁵"}, {'6', "⁶"}, {'7', "⁷"}, {'8', "⁸"}, {'9', "⁹"}};
        for (char c : number) {
            result.append(tinys[c]);
        }
        return result + "⋅";
    }

    void report_move(Position p1, Position p2) {
        auto* moving_piece = find_piece(p1);
        auto* to_piece = find_piece(p2);
        std::string report;
        if (moving_piece->get_side() == Side::White) {
            report = int_to_tiny(std::to_string(move_count_));
        }
        std::map<std::string, std::string> figs{{"♚", "ᴷ"}, {"♛", "੧"},
                                                {"♜", "ᴿ"}, {"♝", "ᴮ"},
                                                {"♞", "ᴺ"}, {"♟", ""}};
        auto piece = moving_piece->get_display();
        report += figs.at(piece);
        const std::array<std::string, 8> columns{
            {"ᵃ", "ᵇ", "ᶜ", "ᵈ", "ᵉ", "ᶠ", "ᵍ", "ʰ"}};
        const std::array<std::string, 8> rows{
            {"¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸"}};
        std::string capture;
        if (to_piece != nullptr) {
            capture = "ˣ";
        }
        report.append(capture)
            .append(columns[p2.column - 1])
            .append(rows[p2.row - 1]);
        if (moving_piece->get_side() == Side::Black) {
            report.append(1, '\n');
        } else {
            report.append(1, ' ');
        }
        this->move_message_sig(report);
    }

    void reset_board() {
        current_side_ = Side::White;
        pieces_.clear();
        pieces_.reserve(32);
        pieces_.push_back(std::make_unique<Pawn>(Side::White, Position{2, 1}));
        pieces_.push_back(std::make_unique<Pawn>(Side::White, Position{2, 2}));
        pieces_.push_back(std::make_unique<Pawn>(Side::White, Position{2, 3}));
        pieces_.push_back(std::make_unique<Pawn>(Side::White, Position{2, 4}));
        pieces_.push_back(std::make_unique<Pawn>(Side::White, Position{2, 5}));
        pieces_.push_back(std::make_unique<Pawn>(Side::White, Position{2, 6}));
        pieces_.push_back(std::make_unique<Pawn>(Side::White, Position{2, 7}));
        pieces_.push_back(std::make_unique<Pawn>(Side::White, Position{2, 8}));
        pieces_.push_back(std::make_unique<Rook>(Side::White, Position{1, 1}));
        pieces_.push_back(
            std::make_unique<Knight>(Side::White, Position{1, 2}));
        pieces_.push_back(
            std::make_unique<Bishop>(Side::White, Position{1, 3}));
        pieces_.push_back(std::make_unique<King>(Side::White, Position{1, 4}));
        pieces_.push_back(std::make_unique<Queen>(Side::White, Position{1, 5}));
        pieces_.push_back(
            std::make_unique<Bishop>(Side::White, Position{1, 6}));
        pieces_.push_back(
            std::make_unique<Knight>(Side::White, Position{1, 7}));
        pieces_.push_back(std::make_unique<Rook>(Side::White, Position{1, 8}));

        pieces_.push_back(std::make_unique<Pawn>(Side::Black, Position{7, 1}));
        pieces_.push_back(std::make_unique<Pawn>(Side::Black, Position{7, 2}));
        pieces_.push_back(std::make_unique<Pawn>(Side::Black, Position{7, 3}));
        pieces_.push_back(std::make_unique<Pawn>(Side::Black, Position{7, 4}));
        pieces_.push_back(std::make_unique<Pawn>(Side::Black, Position{7, 5}));
        pieces_.push_back(std::make_unique<Pawn>(Side::Black, Position{7, 6}));
        pieces_.push_back(std::make_unique<Pawn>(Side::Black, Position{7, 7}));
        pieces_.push_back(std::make_unique<Pawn>(Side::Black, Position{7, 8}));
        pieces_.push_back(std::make_unique<Rook>(Side::Black, Position{8, 1}));
        pieces_.push_back(
            std::make_unique<Knight>(Side::Black, Position{8, 2}));
        pieces_.push_back(
            std::make_unique<Bishop>(Side::Black, Position{8, 3}));
        pieces_.push_back(std::make_unique<King>(Side::Black, Position{8, 4}));
        pieces_.push_back(std::make_unique<Queen>(Side::Black, Position{8, 5}));
        pieces_.push_back(
            std::make_unique<Bishop>(Side::Black, Position{8, 6}));
        pieces_.push_back(
            std::make_unique<Knight>(Side::Black, Position{8, 7}));
        pieces_.push_back(std::make_unique<Rook>(Side::Black, Position{8, 8}));
    }
};

#endif  // MANAGER_HPP
