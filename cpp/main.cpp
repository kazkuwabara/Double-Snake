#include "ctrl.h"

int main() {
    auto game = Ctrl::getInstance();

    // frame rate
    game->setFPS(60.0);

    // Set the time interval between each movement of the snake.
    game->setMoveInterval(150);

    // Set map size, min. 5x5
    game->setGridRow(20);
    game->setGridCol(20);

    return game->run();
}
