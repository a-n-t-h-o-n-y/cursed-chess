cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

add_executable(chess 
    src/main.cpp
    src/chessboard_widget.cpp
    src/position.cpp
    src/chess_engine.cpp
    src/rules.cpp
    src/state.cpp
    src/chess_ui.cpp
    src/side_pane.cpp
    src/settings_pane.cpp
    src/lower_pane.cpp
    src/move_input.cpp
    src/piece.cpp
    src/move.cpp
    src/standard_rules.cpp
    src/no_rules.cpp
    src/shared_user_input.cpp
    src/player_human.cpp
    src/chess_event_loop.cpp
    src/shared_move.cpp
)

target_link_libraries(chess /usr/local/lib64/liblibcppurses.a ncurses)
target_compile_features(chess PUBLIC cxx_std_17)
target_compile_options(chess PUBLIC -Wall)
