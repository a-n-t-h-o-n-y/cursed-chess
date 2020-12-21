#ifndef CHESS_SIDE_HPP
#define CHESS_SIDE_HPP

namespace chess {

// TODO
// remove None and use an optional when you can expect to have a blank space?
// Then a piece holds a Side and it can't be none, and it isn't going to be an
// optional, so anything that is using a Piece object can be guaranteed that
// they have an actual Side to work with. and switch statement can be used.
enum class Side { None, White, Black };

}  // namespace chess
#endif  // CHESS_SIDE_HPP
