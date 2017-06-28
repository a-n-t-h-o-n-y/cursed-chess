cmake_minimum_required(VERSION 3.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(compile_commands ALL
	COMMAND [ -f ${CMAKE_SOURCE_DIR}/build/compile_commands.json ] 
        && mv ${CMAKE_SOURCE_DIR}/build/compile_commands.json
		${CMAKE_SOURCE_DIR}/compile_commands.json || :)

set(CMAKE_CXX_FLAGS "-std=c++14 -g -Wall")

include_directories("/usr/local/include/cppurses")

set(sources "src/main.cpp")

add_executable(chess ${sources})

target_link_libraries(chess /usr/local/lib64/libcppurses.a ncurses)
