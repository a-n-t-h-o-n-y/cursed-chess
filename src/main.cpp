#include "chess_ui.hpp"

#include <cppurses/cppurses.hpp>

using namespace cppurses;
int main() {
    System sys;

    Chess_UI w;

    sys.set_head(&w);
    return sys.run();
}
