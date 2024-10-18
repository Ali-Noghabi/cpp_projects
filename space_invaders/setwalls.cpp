#include "Game.h"

extern Game *game;
void set_walls(){
    Wall *wall2 = new Wall(40, 400);
    game->scene->addItem(wall2);
}
