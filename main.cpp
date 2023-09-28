#include "Game.h"
#include <iostream>

int main()
{
    Game game;

    while (game.running())
    {
        game.updateEvents();
        game.renderFrame();

    }

    return 0;
}