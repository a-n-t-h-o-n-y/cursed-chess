#include <cppurses/cppurses.hpp>

#include "chess_ui.hpp"

using namespace cppurses;
int main() {
    System sys;

    Chess_UI w;

    sys.set_head(&w);
    return sys.run();
}
