#include "Game.h"

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

/*
 * появление клиента
 * говорит свой заказ
 * набор заказа
 * кнопка переспросить заказ
 * кнопка следующий (после этого клиент уменьшается и ставится в очередь)

 */