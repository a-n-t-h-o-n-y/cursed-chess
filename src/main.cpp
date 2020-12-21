#include <cppurses/system/system.hpp>

#include "chess_ui.hpp"

int main() {
    using namespace cppurses;
    System sys;
    chess::Chess_UI w;
    sys.set_head(&w);
    return sys.run();
}
