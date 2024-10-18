#include <Fire.h>
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <Player.h>
#include "Game.h"
#include <Wall.h>

Fire::Fire()
{
    setPixmap(QPixmap(":/images/fire.png"));
    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer -> start(15);
}

extern Game *game;
void Fire::move()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();
    for(int i = 0, n = colliding_items.size(); i < n; i++){
        if(typeid(*(colliding_items[i]))==typeid(Wall)){
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            delete colliding_items[i];
            delete this;
            return;
        }
        if(typeid(*(colliding_items[i]))==typeid(Player)){
            game->health->decrease();
            if (game->health->getHealth() == 0){
                scene()->removeItem(colliding_items[i]);
                delete colliding_items[i];
                QGraphicsTextItem *text = new QGraphicsTextItem();
                text->setPlainText(QString("Game Over !"));
                text->setDefaultTextColor(Qt::red);
                text->setFont(QFont("times", 34));
                text->setPos(240, 250);
                scene()->addItem(text);
            }
            scene()->removeItem(this);
            delete this;
            return;
        }
    }
    setPos(x(), y()+3);
    if(pos().y() > 600){
        scene()->removeItem(this);
        delete this;
    }
}
