cmake_minimum_required(VERSION 3.14)

add_executable(cursed-chess
    chess_engine.cpp
    chess_event_loop.cpp
    chess_ui.cpp
    chessboard_widget.cpp
    main.cpp
    move.cpp
    move_input.cpp
    no_rules.cpp
    piece.cpp
    position.cpp
    rules.cpp
    shared_move.cpp
    shared_user_input.cpp
    standard_rules.cpp
    state.cpp
)

target_link_libraries(
    cursed-chess
    PRIVATE
        cppurses
)

target_compile_features(
    cursed-chess
    PRIVATE
        cxx_std_17
)

target_compile_options(
    cursed-chess
    PRIVATE
        -Wall
        -Wextra
)
