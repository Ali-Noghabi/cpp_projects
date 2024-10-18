#include <Wall.h>


Wall::Wall(int x, int y, QGraphicsItem *parent)
{
    setPixmap(QPixmap(":/images/wall.png"));
    setPos(x, y);
}
