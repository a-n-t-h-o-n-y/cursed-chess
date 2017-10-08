cmake_minimum_required(VERSION 3.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(compile_commands ALL
	COMMAND [ -f ${CMAKE_SOURCE_DIR}/build/compile_commands.json ] 
        && mv ${CMAKE_SOURCE_DIR}/build/compile_commands.json
		${CMAKE_SOURCE_DIR}/compile_commands.json || :)

set(CMAKE_CXX_FLAGS "-std=c++14 -g -Wall")

include_directories("/usr/local/include/cppurses")

set(SOURCES "src/main.cpp" "src/chessboard_widget.cpp" "src/position.cpp"
    "src/chess_engine.cpp" "src/invalid_position.cpp" "src/rules.cpp"
    "src/state.cpp" "src/chessboard_borders.cpp" "src/chess_full_ui.cpp"
    "src/side_pane.cpp" "src/log.cpp"
    )

add_executable(chess ${SOURCES})

target_link_libraries(chess /usr/local/lib64/libcppurses.a ncurses)
