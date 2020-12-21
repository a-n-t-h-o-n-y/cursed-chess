cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_executable(chess
    src/chess_engine.cpp
    src/chess_event_loop.cpp
    src/chess_ui.cpp
    src/chessboard_widget.cpp
    src/main.cpp
    src/move.cpp
    src/move_input.cpp
    src/no_rules.cpp
    src/piece.cpp
    src/position.cpp
    src/rules.cpp
    src/shared_move.cpp
    src/shared_user_input.cpp
    src/standard_rules.cpp
    src/state.cpp
)

target_link_libraries(chess /usr/local/lib64/liblibcppurses.a ncurses)
target_compile_features(chess PUBLIC cxx_std_17)
target_compile_options(chess PUBLIC -Wall)
