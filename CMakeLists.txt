cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
# set(CMAKE_BUILD_TYPE RELEASE)

add_executable(chess 
    src/main.cpp
    src/chessboard_widget.cpp
    src/position.cpp
    src/chess_engine.cpp
    src/rules.cpp
    src/state.cpp
    src/chessboard_borders.cpp
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
    src/chess_move_request_event.cpp
)

target_link_libraries(chess /usr/local/lib64/liblibcppurses.a ncurses)
target_compile_features(chess PUBLIC cxx_std_14)
target_compile_options(chess PUBLIC -Wall)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(compile_commands ALL
	COMMAND [ -f ${CMAKE_SOURCE_DIR}/build/compile_commands.json ] 
        && mv ${CMAKE_SOURCE_DIR}/build/compile_commands.json
		${CMAKE_SOURCE_DIR}/compile_commands.json || :)

