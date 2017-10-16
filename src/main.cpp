#include "chess_full_ui.hpp"

#include <cppurses/cppurses.hpp>

using namespace cppurses;
int main() {
    System sys;

    Chess_full_UI w;

    sys.set_head(&w);
    return sys.run();
}
